/**
 * task.c
 * Управление потоками
 *
 * Данный файл содержит функции для создания и удаления потоков.
 *
 */

#include <arch.h>
#include <fb.h>
#include <log.h>
#include <mem.h>

static volatile uint64_t next_thread_id = 0;
static task_t *last_task = NULL;
static task_t *kernel_task = NULL;
task_t *current_task = NULL;
uint32_t *test_buf = NULL;
extern uint64_t full_init;
uint64_t task_f_init = 0;
lock_t task_lock;

void task_switch_asm(task_t *, task_t *);

void task_switch( ) {
	asm volatile("cli");

	task_t *next = current_task->next;
	task_t *last = current_task;

	if (current_task->cpu_time_expired) {
		current_task->cpu_time_expired--;
		outb(0x20, 0x20);
		task_switch_asm(current_task, current_task);
		return;
	}

	current_task->cpu_time_expired = current_task->cpu_time;

	current_task = next;

	// LOG("Смена потоков %u->%u\n", last->id, next->id);
	outb(0x20, 0x20);
	task_switch_asm(last, next);
}

uint64_t task_new_thread(void (*func)(void *), char *name) {
	LOG("Выделение потока\n");

	uint64_t cr3;
	uint64_t *stack = mem_alloc(STACK_SIZE);
	task_t *new_task = mem_alloc(sizeof(task_t));

	asm volatile("mov %%cr3, %0" : "=r"(cr3));

	tool_memset(stack, 0, STACK_SIZE);
	tool_memset(new_task, 0, sizeof(task_t));

	new_task->stack = stack;

	uint64_t stack_top = STACK_SIZE;
	stack[--stack_top] = (uint64_t)stack;
	stack[--stack_top] = (uint64_t)func;
	stack[--stack_top] = (uint64_t)0;

	new_task->rsp = (uint64_t)new_task->stack + sizeof(uint64_t) * stack_top;
	new_task->cpu_time = 500;
	new_task->cpu_time_expired = new_task->cpu_time;
	new_task->id = next_thread_id++;
	new_task->id_str = name;
	new_task->cr3 = cr3;

	new_task->last = current_task;
	new_task->next = current_task->next;
	current_task->next->last = new_task;
	current_task->next = new_task;

	LOG("Создан новый поток с ID: %u\n", new_task->id);

	if (full_init == 0) { current_task = new_task; }

	return new_task->id;
}

void task_del(uint64_t id) {
	task_t *task = current_task;

	// Поиск задачи по ID
	while (task->id != id) {
		task = task->next;

		// Если вернулись к начальной задаче, значит задачи с данным ID не существует
		if (task == current_task) {
			LOG("Задача с ID %u не существует\n", id);
			return;
		}
	}

	LOG("Удаление потока ID: %u, %s\n", current_task->id, current_task->id_str);
	task_t *prev = task->last;
	task_t *next = task->next;

	prev->next = next;
	next->last = prev;

	mem_free(task->stack);
	mem_free(task);

	if (task == current_task) {
		current_task = next;
		if (full_init) { task_switch( ); }
	}
}

void task_del_current( ) {
	LOG("Удаление потока ID: %u, %s\n", current_task->id, current_task->id_str);
	task_t *prev = current_task->last;
	task_t *next = current_task->next;

	prev->next = next;
	next->last = prev;

	mem_free(current_task->stack);
	mem_free(current_task);

	current_task = next;
	if (full_init) { task_switch( ); }
}

void task_after_init( ) {
	if (full_init) {
		current_task = kernel_task;
		kernel_task->id_str = "[KERNEL]";
	}
}

void task_init( ) {
	asm volatile("cli");
	idt_set_int(32, task_switch);

	uint64_t rsp;
	uint64_t cr3;

	LOG("Создание потока ядра\n");
	asm volatile("mov %%rsp, %0" : "=r"(rsp));
	asm volatile("mov %%cr3, %0" : "=r"(cr3));

	LOG("Настройка потока ядра\n");
	// mem_dump_memory( );
	task_t *new_task = mem_alloc(sizeof(task_t));
	LOG("%x\n", new_task);
	kernel_task = new_task;

	tool_memset(kernel_task, 0, sizeof(task_t));

	kernel_task->id = next_thread_id++;
	kernel_task->id_str = "kernel_early";
	kernel_task->rsp = rsp;
	kernel_task->cr3 = cr3;
	kernel_task->cpu_time = 100;
	kernel_task->cpu_time_expired = kernel_task->cpu_time;

	current_task = kernel_task;

	current_task->last = current_task;
	current_task->next = current_task;

	last_task = kernel_task;

	LOG("Потоки инициализированы\n");
}
