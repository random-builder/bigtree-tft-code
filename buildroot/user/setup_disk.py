#!/usr/bin/env python

#
# copy firmware to remote octopi host with marlin usb connection
#

import os
import sys
import time
import logging
import paramiko
import argparse
import functools
import subprocess

logging.basicConfig(
    level=logging.INFO,
    datefmt='%Y-%m-%d %H:%M:%S',
    format='%(asctime)s %(levelname)-8s %(message)s',
)

logger = logging.getLogger(__name__)

parser = argparse.ArgumentParser()
parser.add_argument('--user_name', dest="user_name", default="pi")
parser.add_argument('--host_name', dest="host_name", default="octopi2")
parser.add_argument('--disk_label', dest="disk_label", default="MKS-BASE")
parser.add_argument('--firmware_path', dest="firmware_path", required=True)

params = parser.parse_args()

logger.info(f"params report:")
for key, value in vars(params).items():
    logger.info(f"   {key}={value}")


def trace(func):

    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        id = func.__name__
        try:
            logger.info(f"{id}")
            return func(*args, *kwargs)
        except Exception as error:
            logger.exception(error)
            raise error

    return wrapper


@trace
def disk_find_name(label:str) -> str:
    "resolve disk label into disk name"
    result = invoke_ssh(f"sudo lsblk -n -l -o name,label | grep {label} | head -n 1")
    if result.returncode == 0:
        part_list = result.stdout.split()
        if len(part_list) == 2:
            disk_name = part_list[0].decode('utf-8')
            return disk_name
        else:
            return None
    else:
        return None


@trace
def disk_has_point(disk_point:str) -> bool:
    "verify if mount point is present"
    result = invoke_ssh(f"cat /proc/mounts | grep {disk_point}")
    return result.returncode == 0


@trace
def disk_point_create(disk_point) -> None:
    invoke_ssh(f"mkdir -p {disk_point}")


@trace
def disk_point_delete(disk_point) -> None:
    invoke_ssh(f"rm -rf {disk_point}")


@trace
def disk_file_copy(local_path:str, remote_dir:str) -> None:
    local_base = os.path.basename(local_path)
    source_path = local_path
    target_path = os.path.join(remote_dir, local_base)
    invoke_scp(source_path, target_path)


@trace
def disk_point_mount(disk_point:str, disk_path:str) -> None:
    if disk_has_point(disk_point) is False:
        invoke_ssh(f"sudo mount {disk_path} {disk_point} -o uid=$USER,gid=$USER")


@trace
def disk_point_unmount(disk_point:str) -> None:
    if disk_has_point(disk_point) is True:
        invoke_ssh(f"sudo umount {disk_point}")


@trace
def disk_setup(disk_name:str, firmware_path:str) -> None:
    disk_path = f"/dev/{disk_name}"
    disk_point = f"/tmp/disk-point/{disk_name}"
    disk_point_unmount(disk_point)
    disk_point_create(disk_point)
    disk_point_mount(disk_point, disk_path)
    disk_file_copy(firmware_path, disk_point)
    disk_point_unmount(disk_point)
    disk_point_delete(disk_point)


def shell(script):
    result = subprocess.run(script, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    return result


def has_host(host_name):
    result = shell(f"ping -c 1 {host_name}")
    return result.returncode == 0


class Result() :
    returncode = 0
    stdout = ""
    stderr = ""


def invoke_ssh(script):
    stdin, stdout, stderr = ssh_client.exec_command(script)
    stdin.flush()
    result = Result()
    result.returncode = stdout.channel.recv_exit_status()
    result.stdout = stdout.read()
    result.stderr = stderr.read()
    return result


def invoke_scp(source_path, target_path):
    ftp_client = ssh_client.open_sftp()
    ftp_client.put(source_path, target_path)
    ftp_client.close()

#
#
#


if not has_host(params.host_name):
    logger.info(f"missing host={params.host_name}")
    sys.exit()

ssh_client = paramiko.SSHClient()
ssh_client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh_client.connect(username=params.user_name, hostname=params.host_name,)

disk_name = disk_find_name(params.disk_label)

if disk_name is None:
    logger.info(f"missing disk={params.disk_label}")
    sys.exit()

disk_setup(disk_name, params.firmware_path)
