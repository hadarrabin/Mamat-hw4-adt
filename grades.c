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
typedef struct list list;
typedef struct iterator iterator;

typedef struct{
    char *course_name;
    int course_grade;
} Grade;

typedef struct{
    char *name;
    int id;
    struct list* grades_list;
    float average;
    int number_courses;
} Student;

/**
 * @brief Creates a new Grade structure
 * @param course_name The name of the course
 * @param course_grade The grade for the course (0-100)
 * @return Pointer to the newly created Grade structure, or NULL if creation failed
 */
Grade grade_create(char *course_names, int course_grades) {
    if(course_grades < 0 || course_grades > 100) {
        return Null;
    }
    Grade *new_grade = (Grade)malloc(sizeof(Grade));
    if(!new_grade) {
        return Null;
    }
    new_grade->course_name = malloc(sizeof(char)(strlen(course_names)+1));
    if(!new_grade->course_name) {
        free(new_grade);
        return Null;
    }
    strcpy(new_grade->course_name, course_names);

    new_grade->course_grade = course_grades;
    return new_grade;
}


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

void grade_destroy(elem_t grade) {
    grade *g = (Grade) grade;
    free(g->course_name);
    free(g);
}



Student student_create(int id, char *name, list *grades) {
    if(!name || !grades) {
        return Null;
    }
    Student *s = (Student)malloc(sizeof(Student));
    if(!s) {
        return Null;
    }
    s->name = malloc(sizeof(char)*(strlen(name)+1));
    if(!s->name) {
        return Null;
    }
    strcpy(s->name, name);
    s->id = id;
    s->grades_list = grades;
}

int student_clone(elem_t student_in, elem_t *student_out) {
    if(!input || !output) {
        return Failure;
    }
    Student *in = (Student) student_in;
    Student *out;
    out->grades_list =
    out = Student_create(input->course_name,input->course_grade);
    if(!out) {
        output = null;
        return Failure;
    }
    *output = out;
    return Success;
}

