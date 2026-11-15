#include <stdlib.h>
#include "memory.h"
#include <stdio.h>
#include <stdint.h>
#include "hardware/regs/addressmap.h"
#include "pico/stdlib.h"
#include "command.h"
#include "device.h"

extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;

int main(void);

uint32_t data_variable = 100;
uint32_t bss_variable;

static void row(const char *name, uintptr_t start, uintptr_t end)
{
    printf("%-10s 0x%08x 0x%08x %8u\n",
           name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}

void mem_info(void)
{
    printf("%-10s %-10s %-10s %8s\n", "area", "start", "end", "size");

    uintptr_t flash_start = XIP_BASE;
    uintptr_t flash_end = XIP_BASE + PICO_FLASH_SIZE_BYTES;
    uintptr_t sram_start = SRAM_BASE;
    uintptr_t sram_end = SRAM_BASE + 264 * 1024;
    uintptr_t rom_start = ROM_BASE;
    uintptr_t rom_end = ROM_BASE + 16 * 1024;

    row("flash", flash_start, flash_end);
    row("sram", sram_start, sram_end);
    row("rom", rom_start, rom_end);
    
    row("image", (uintptr_t)&__flash_binary_start, (uintptr_t)&__flash_binary_end);
    row("free", (uintptr_t)&__flash_binary_end, flash_end);
    row("boot2", (uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__);
    row("text", (uintptr_t)&__boot2_end__, (uintptr_t)&__etext);

    uintptr_t data_size = (uintptr_t)&__data_end__ - (uintptr_t)&__data_start__;

    row("data flash", (uintptr_t)&__etext, (uintptr_t)&__etext + data_size);
    row("data ram", (uintptr_t)&__data_start__, (uintptr_t)&__data_end__);
    row("bss", (uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__);
    row("heap", (uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit);
    row("stack", (uintptr_t)&__StackBottom, (uintptr_t)&__StackTop);

    printf("\ntotal\n");
    
    unsigned image_size = (unsigned)((uintptr_t)&__flash_binary_end - (uintptr_t)&__flash_binary_start);
    unsigned boot2_size = (unsigned)((uintptr_t)&__boot2_end__ - (uintptr_t)&__boot2_start__);
    unsigned text_size = (unsigned)((uintptr_t)&__etext - (uintptr_t)&__boot2_end__);
    unsigned bss_size = (unsigned)((uintptr_t)&__bss_end__ - (uintptr_t)&__bss_start__);
    unsigned heap_size = (unsigned)((uintptr_t)&__HeapLimit - (uintptr_t)&__bss_end__);
    unsigned stack_size = (unsigned)((uintptr_t)&__StackTop - (uintptr_t)&__StackBottom);

    printf("  flash image   %8u = boot2 %u + text %u + data %u\n", image_size, boot2_size, text_size, (unsigned)data_size);
    printf("  flash free    %8u of %u\n", PICO_FLASH_SIZE_BYTES - image_size, PICO_FLASH_SIZE_BYTES);
    printf("  ram used      %8u = data %u + bss %u\n", (unsigned)data_size + bss_size, (unsigned)data_size, bss_size);
    printf("  ram free      %8u for heap and %u for stack\n", heap_size, stack_size);
}

void fw_info(void)
{

    data_variable++;
    bss_variable++;

    
    uintptr_t main_addr = (uintptr_t)main;
    uintptr_t main_code_addr = main_addr & ~1u;
    uint16_t main_code = *(uint16_t *)main_code_addr;

    uintptr_t fw_info_addr = (uintptr_t)fw_info;
    uintptr_t fw_info_code_addr = fw_info_addr & ~1u;
    uint16_t fw_info_code = *(uint16_t *)fw_info_code_addr;

    
    uint32_t stack_variable = 1946;
    uint32_t *heap_variable = malloc(sizeof(uint32_t));

    if (heap_variable != NULL)
    {
        *heap_variable = 1951;
    }

    
    printf("%-15s %-10s %s\n", "object", "address", "value");
    
  
    printf("%-15s 0x%08x  0x%04x\n", "main", (unsigned)main_addr, main_code);
    printf("%-15s 0x%08x  0x%04x\n", "fw_info", (unsigned)fw_info_addr, fw_info_code);
    
    
    printf("%-15s 0x%08x\n", "commands", (unsigned)(uintptr_t)commands);
    for (uint i = 0; i < command_count; i++)
    {
        printf("- %-13s 0x%08x\n", commands[i].name, (unsigned)(uintptr_t)commands[i].handler);
    }

    
    printf("%-15s 0x%08x  %s\n", "DEVICE_PROJECT", (unsigned)(uintptr_t)DEVICE_PROJECT, DEVICE_PROJECT);
    printf("%-15s 0x%08x  %s\n", "DEVICE_BOARD", (unsigned)(uintptr_t)DEVICE_BOARD, DEVICE_BOARD);

    
    printf("%-15s 0x%08x  %u\n", "data_variable", (unsigned)(uintptr_t)&data_variable, (unsigned)data_variable);
    printf("%-15s 0x%08x  %u\n", "bss_variable", (unsigned)(uintptr_t)&bss_variable, (unsigned)bss_variable);
    printf("%-15s 0x%08x  %u\n", "stack_variable", (unsigned)(uintptr_t)&stack_variable, (unsigned)stack_variable);
    
    if (heap_variable != NULL)
    {
        printf("%-15s 0x%08x  %u\n", "heap_variable", (unsigned)(uintptr_t)heap_variable, (unsigned)*heap_variable);
        free(heap_variable); 
    }
}