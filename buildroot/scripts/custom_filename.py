Import("env")

build_flags = env.ParseFlags(env['BUILD_FLAGS'])

print(f"SCRIPTS build_flags: {build_flags}")

cpp_defines = build_flags.get("CPPDEFINES")

print(f"SCRIPTS cpp_defines: {cpp_defines}")

flags = { k: v for (k, v) in cpp_defines }

print(f"SCRIPTS flags: {flags}")

filename = flags.get("HARDWARE") + "." + flags.get("SOFTWARE_VERSION")

print(f"SCRIPTS filename: {filename}")

env.Replace(PROGNAME=filename)
