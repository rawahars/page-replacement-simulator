/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "input_parser.h"

char *extractLineFromFile(FILE *file, long file_ptr);

char *reallocateString(char *buffer, int len);

memory_reference *split(char *str, int initialIndex, int maxLen, long file_ptr);

memory_reference *ReadLine(FILE *file, long file_ptr) {
    //Extract the line from the file
    char *line = extractLineFromFile(file, file_ptr);
    if (strcmp(line, "") == 0)
        return NULL;
    //Extract memory reference from the line
    memory_reference *reference = split(line, 0, strlen(line), file_ptr);
    return reference;
}

memory_reference *CreateMemoryReference(int pid, int vpn, long file_ptr) {
    memory_reference *mem_ref = malloc(sizeof(memory_reference));
    ValidateMemoryAllocationError(mem_ref);
    mem_ref->file_ptr = file_ptr;
    mem_ref->pid = pid;
    mem_ref->vpn = vpn;
    return mem_ref;
}

/**
 * This method is used to extract a line from a file. We perform buffer overflow and null byte check as we extract the line.
 * This method has been reused from our last assignment.
 * */
char *extractLineFromFile(FILE *file, long file_ptr) {
    char *buffer = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    ValidateMemoryAllocationError(buffer);
    int len = 0;
    char ch;

    while (1) {
        ch = fgetc(file);

        if (len >= MAX_BUFFER_SIZE) { // During buffer overflow exit with error
            buffer[MAX_BUFFER_SIZE - 1] = '\0';
            BufferOverflowError(file_ptr, buffer);
        } else if (ch == '\0') { // If a null byte is found in line then exit with error
            buffer[len] = '\0';
            NullByteInLineError(file_ptr, buffer);
        } else if (ch == EOF || ch == '\n') {
            buffer[len++] = '\0';
            break;
        } else {
            buffer[len++] = ch;
        }
    }

    char *final_str = reallocateString(buffer, len);
    return final_str;
}

/**
 * This method is used to reallocate a string to its size
 * This method has been reused from our last assignment.
 * */
char *reallocateString(char *buffer, int len) {
    char *final_str = malloc(sizeof(char) * len);
    ValidateMemoryAllocationError(final_str);
    strcpy(final_str, buffer);
    free(buffer);
    return final_str;
}

/**
 * This method is used to split the line on whitespaces for getting (pid,vpn)
 * */
memory_reference *split(char *str, int initialIndex, int maxLen, long file_ptr) {
    memory_reference *ref = CreateMemoryReference(0, 0, file_ptr);
    int index;
    int num_count = 0;
    char *curr = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    ValidateMemoryAllocationError(curr);
    int curr_index = 0;

    for (index = initialIndex; index <= maxLen; index++) {
        if (index == maxLen || str[index] == ' ' || str[index] == '\t' || str[index] == '\r') {
            //Record this num
            if (curr_index > 0) {
                curr[curr_index] = '\0';
                if (num_count == 0)
                    ref->pid = atoi(curr);
                else if (num_count == 1)
                    ref->vpn = atoi(curr);
                else
                    InvalidInputError();

                num_count++;
                memset(curr, '\0', MAX_BUFFER_SIZE);
            }
            curr_index = 0;
            continue;
        }
        if (!isdigit(str[index - initialIndex]))
            InvalidInputError();
        curr[curr_index++] = str[index - initialIndex];
    }

    free(curr);
    return ref;
}

memory_reference *ReadLineAtIndex(FILE *file, long file_ptr) {

    fseek(file, file_ptr, SEEK_SET);
    char *line = extractLineFromFile(file, file_ptr);
    memory_reference *reference = split(line, 0, strlen(line), file_ptr);
    return reference;
}