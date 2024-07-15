//
// Created by hadar and tamar on 7/15/2024.
//

//includes
#include "grades.h"
#include "linked-list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Result { Success = 0, Failure };

typedef void* elem_t;


typedef struct{
    char *course_name;
    int course_grade;
} Grade;


/**
*@brief
*/
int grade_clone(elem_t input, elem_t *output) {
    if(!input || !output)    {
        output = null;
        return Failure;
    }
    Grade *g = (Grade) input;
    Grade *out;
    out = grade_create(input->course_name,input->course_grade);
    if(!out) {
        output = null;
        return Failure;
    }
    *output = out;
    return Success;
}

Grade grade_create(char *course_names, int course_grades) {
    Grade new_grade = (Grade)malloc(sizeof(Grade));
    if(!new_grade) {
        return null;
    }
    new_grade->course_name = malloc(sizeof(char)(strlen(course_names)+1));
    if(!new_grade->course_name) {
        return null;
    }
    strcpy(new_grade->course_name, course_names);
    new_grade->course_grade = course_grades;
}

