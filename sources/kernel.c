#include "manipulator/asm_bus.h"
#include "vga/cursor.h"
#include "vga/write.h"
#include "port/keyboard.h"
#include "kernel/component_manager.h"
#include "kernel/memory_manager.h"
#include "vga/vga.h"
#include "header/flaginfo.h"
#include "type/string.h"

int PANIC_FLAG = 0;

int k_test_init()
{
    TestComponent* test_c = (TestComponent*)k_component_get(0);
    if(!test_c) return COMPONENT_NULL;

    TestComponent* test_c2 = (TestComponent*)k_component_get(0);

    while(test_c->randomint32 != 0x9438)
    {
        test_c2->randomint32++;
    }

    return 0;
}

// K_ prefix means that only the kernel has access to this function
void k_kernel_panic()
{
    VgaComponent* vga_c = (VgaComponent*)k_component_get(1);
    int position = !vga_c ? 0 : vga_c->cursor_position;

    old_print(&position, 0x0C, "\n\nKERNEL PANIC!!!!!!!!!!!!!!!!!!!!\n");
    old_print(&position, 0x0F, "Press any key to reboot...");
    update_cursor(position);
    PANIC_FLAG = true;
}

void main()
{
    enable_cursor(10,13);
    k_init_component_manager();
    

    TestComponent* test = (TestComponent*)k_component_get(0);

    VgaComponent* vga = (VgaComponent*)k_component_get(1);
    if(!vga) k_kernel_panic();
    int COLOR = vga->color;

    k_memory_init();
    const char* testAlloc = (const char*) malloc(7 * sizeof(char)); 
    if(!testAlloc) k_kernel_panic();
    strncpy((char*)testAlloc, "retete", 7);

    print(testAlloc);
    free((void*)testAlloc);

    while(!PANIC_FLAG)
    {
        char key = read_keyboard_buffer();
        if(key == 'S') k_kernel_panic();
        if(key == '\\')
        {
            test->randomint32 = 0x9438;
            print("\nRANDOM VALUE CHANGED TO: 0x9438\n");
            int res = k_test_init();
            if(res == COMPONENT_NULL) print("COMPONENT NULL\n");
            if(res == COMPONENT_INVALID_VALUE) print("COMPONENT INVALID VALUE\n");
            if(res == 0) print("COMPONENT OK\n");
            
            continue;
        } 
        if(key == 0x0) continue;
        
        // horrible way but works lmao
        int CURSOR_POSITION = vga->cursor_position;
        k_print_char(&CURSOR_POSITION, COLOR, key); 
        update_cursor(CURSOR_POSITION);
        vga->cursor_position = CURSOR_POSITION;
    }
    
    return;
}
