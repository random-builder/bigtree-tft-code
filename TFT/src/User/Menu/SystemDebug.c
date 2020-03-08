//
// system development support
//

#include <malloc.h>

#include "includes.h"

// report memory allocator status
// see https://linux.die.net/man/3/mallinfo
// see https://www.gnu.org/software/libc/manual/html_node/Statistics-of-Malloc.html
void menu_system_memory_report() {

    char text_line[64];
    struct mallinfo memory_info;

    memory_info = mallinfo();

    GUI_Clear(BACKGROUND_COLOR);

    GUI_DispString(0, BYTE_HEIGHT * 1, (u8*) "Memory Report (mallinfo)");

    my_sprintf(text_line, "Total size (arena)    : %d", memory_info.arena);
    GUI_DispString(0, BYTE_HEIGHT * 2, (u8*) text_line);

    my_sprintf(text_line, "Total used (uordblks) : %d", memory_info.uordblks);
    GUI_DispString(0, BYTE_HEIGHT * 3, (u8*) text_line);

    my_sprintf(text_line, "Total free (fordblks) : %d", memory_info.fordblks);
    GUI_DispString(0, BYTE_HEIGHT * 4, (u8*) text_line);

    utility_prompt_touch_to_exit();

}
