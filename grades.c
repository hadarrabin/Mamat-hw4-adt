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
typedef struct iterator* iter;

typedef struct{
    char *course_name;
    int course_grade;
} Grade;

typedef struct{
    char *name;
    int id;
    list* grades_list;
    float average;
    int number_courses;
} Student;

/**                 Grade user-functions */

/**
 * @brief Creates a new Grade structure
 * @param course_name The name of the course
 * @param course_grade The grade for the course (0-100)
 * @return Pointer to the newly created Grade structure, or NULL if creation failed
 */
Grade* grade_create(char *course_name, int course_grade) {
    if(course_grade < 0 || course_grade > 100) {
        return NULL;
    }
    Grade *new_grade = (Grade*)malloc(sizeof(Grade));
    if(!new_grade) {
        return NULL;
    }
    new_grade->course_name = malloc(sizeof(char)*(strlen(course_name)+1));
    if(!new_grade->course_name) {
        free(new_grade);
        return NULL;
    }
    strcpy(new_grade->course_name, course_name);
    new_grade->course_grade = course_grade;
    return new_grade;
}


/**
 * @brief Creates a copy of Grade structure
 * @param imput a pointer to the grade to clone
 * @param output a pointer to a pointer of the cloned grade
 * @return Pointer to the newly created Grade structure, or NULL if creation failed
 */
int grade_clone(elem_t input, elem_t *output) {
    if(!input || !output)    {
        if (output) {
            *output = NULL;
        }
        return Failure;
    }
    Grade *g = (Grade*) input;
    Grade *out = grade_create(g->course_name,g->course_grade);
    
    if(!out) {
        *output = NULL;
        return Failure;
    }
    *output = out;
    return Success;
}

/**
 * @brief destroyes grade structure
 * @param input a void* pointer to the grade to destroy
 * @return None
 */
void grade_destroy(elem_t grade) {
    Grade *g = (Grade*) grade;
    free(g->course_name);
    free(g);
}

/**
 * @brief Creates a copy of list of grade structure, copeis each element while
 * @brief iterates it.
 * @param dest a pointer to the list of grades we want to copy to
 * @param src a pointer to the list of grades we want to copy from
 * @return None
 */
void duplicate_gradeslist( list *dest, list *src) {
    if (!dest || !src) {
        return;
    }
    iter current_src = list_begin(src);
    elem_t current_dest = NULL;

    while(current_src) {
        if(grade_clone(list_get(current_src), &current_dest) == Failure) {
            return;
        }
        if(!current_dest) {
            return;
        }
        if(list_push_back(dest, current_dest)) {
            //if push is failed then manage memory
            grade_destroy(current_dest);
            return;
        }

        current_src = list_next(current_src);
    }
}


/** Student user-functions       */

Student* student_create(int id, char *name) {
    if(!name ) {
        return NULL;
    }
    Student *s = (Student*)malloc(sizeof(Student));
    if(!s) {
        return NULL;
    }
    s->name = malloc(sizeof(char)*strlen(name)+1);
    if(!s->name) {
		free(s);
        return NULL;
    }
    strcpy(s->name,name);
    s->id = id;
    s->average = 0;
    s->number_courses = 0;
    s->grades_list = list_init(grade_clone, grade_destroy);
    if(!(s->grades_list)) {
        free(s->name);
        free(s);
        return NULL;
    }
    return s;
}


int student_clone(elem_t student_in, elem_t *student_out) {
    if(!student_in || !student_out) {
        return Failure;
    }
    Student *in = (Student*) student_in;
    Student *out = student_create(in->id ,in->name);
    if(!out) {
        *student_out = NULL;
        return Failure;
    }
    duplicate_gradeslist(out->grades_list, in->grades_list);
    *student_out = out;
    return Success;
}

void student_destroy(elem_t student) {
    Student *s = (Student*)student;
    list_destroy(s->grades_list);
    free(s->name);
    free(s);
}

/**         student - end    */

typedef struct grades {
    list *students_list;
} grades;

/**
 * @brief Initializes the "grades" data-structure.
 * @returns A pointer to the data-structure, of NULL in case of an error
 */
struct grades* grades_init() {
    grades *new_grades = (grades*)malloc(sizeof(grades));
    if(!new_grades) {
        return NULL;
    }
    new_grades->students_list = list_init(student_clone, student_destroy);
    return new_grades;
}

/**
 * @brief Destroys "grades", de-allocate all memory!
 */
void grades_destroy(struct grades *grades) {
    list_destroy(grades->students_list);
    free(grades);
}

/**
 * @brief go through the student list and look for an ID
 */
 Student* student_search (list *students, int id) {
    if (!students) {
        return NULL;
    }
    iter student_current = list_begin(students);
    while (student_current != NULL) {
        Student *p_student_current = (Student*)list_get(student_current);
        if (p_student_current->id == id) {
            return p_student_current;
        }
        student_current = list_next(student_current);
    }
    return NULL;
 }

/**
 * @brief Adds a student with "name" and "id" to "grades"
 * @returns 0 on success
 * @note Failes if "grades" is invalid, or a student with
 * the same "id" already exists in "grades"
 */
int grades_add_student(struct grades *grades, const char *name, int id) {
    if (!grades) {
        return Failure;
    }   
    if (student_search(grades->students_list, id)) {
        return Failure;
    }
    Student *new_student = student_create(id, (char *)name);
	if(!new_student) {
		return Failure;
	}
    if (list_push_back(grades->students_list, (elem_t)new_student)) {
        student_destroy(new_student);
        return Failure;
    }
    return Success;
}

/**
 * @brief go through the student's courses list and look for a name
 */
 int course_search ( list* grades_list, const char *name) {
    if (!grades_list || !name) {
        return Failure;
    }
    iter grade_current = list_begin(grades_list);
    while (grade_current) {
        Grade *p_grade_current = (Grade*)list_get(grade_current);
        if (strcmp(p_grade_current->course_name, name) == Success) {
            return Success;
        }
        grade_current = list_next(grade_current);
    }
    return Failure;
 }

/**
 * @brief Adds a course with "name" and "grade" to the student with "id"
 * @return 0 on success
 * @note Failes if "grades" is invalid, if a student with "id" does not exist
 * in "grades", if the student already has a course with "name", or if "grade"
 * is not between 0 to 100.
 */
 int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade) {
                        if (!grades || grade < 0 || grade > 100) {
                            return Failure;
                        }
                        Student *s = student_search(grades->students_list, id);
                        if (!s) {
                            return Failure;
                        }
                        if (course_search(s->grades_list, name) == Success) {
                            return Failure;
                        }
                        else {
                            Grade *g = grade_create((char *)name, grade);
                            if(!g || list_push_back(s->grades_list, (elem_t)g)) {
                                grade_destroy(g);
								return Failure;
                            }
                            s->average = ((s->number_courses) * (s->average) +
                             grade)/ (s->number_courses +1);
                            s->number_courses += 1;
                            return Success;
                        }
                     }

/**
 * @brief Calcs the average of the student with "id" in "grades".
 * @param[out] out This method sets the variable pointed by "out" to the
 * student's name. Needs to allocate memory. The user is responsible for
 * freeing the memory.
 * @returns The average, or -1 on error
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note If the student has no courses, the average is 0.
 * @note On error, sets "out" to NULL.
 */
float grades_calc_avg(struct grades *grades, int id, char **out) {
    if(!grades || !out) {
        if (out) {
            *out = NULL;
        }
        return -1;
    }
    Student *s = student_search(grades->students_list, id);
    if(!s) {
		*out = NULL;
        return -1;
    }
    *out = malloc(sizeof(char)*strlen(s->name)+1);
    if(!(*out)) {
        return -1;
    }
    strcpy(*out, s->name);
    return s->average;
}

/**
 * @brief Prints the courses of the student with "id" in the following format:
 * STUDENT-NAME STUDENT-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note The courses should be printed according to the order 
 * in which they were inserted into "grades"
 */
int grades_print_student(struct grades *grades, int id) {
    if(!grades) {
        return Failure;
    }
    Student *s = student_search(grades->students_list, id);
    if(!s) {
        return Failure;
    }
    printf("%s %d: ", s->name, s->id);
    list *grades_l = s->grades_list;
    if (list_size(grades_l) == 0) {
        printf("\n");
        return Success;
    }
    iter grade_current = list_begin(grades_l);
    while (grade_current) {
        Grade *p_grade_current = (Grade*)list_get(grade_current);
        if (!p_grade_current) {
            return Failure;
        }
        printf("%s %d",p_grade_current->course_name,
		p_grade_current->course_grade);
        if (list_next(grade_current)) {
            printf (", ");
        } else {
            printf("\n");
        }
        grade_current = list_next(grade_current);
    }
    return Success;
}

/**
 * @brief Prints all students in "grade", in the following format:
 * STUDENT-1-NAME STUDENT-1-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * STUDENT-2-NAME STUDENT-2-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid
 * @note The students should be printed according to the order 
 * in which they were inserted into "grades"
 * @note The courses should be printed according to the order 
 * in which they were inserted into "grades"
 */
int grades_print_all(struct grades *grades) {
    if(!grades) {
        return Failure;
    }
    size_t students_num = list_size(grades->students_list);
    iter student_current = list_begin(grades->students_list);
    for(size_t i=0; i<students_num; i++) {
        Student *s = (Student *)list_get(student_current);
        grades_print_student(grades,  s->id);
        student_current = list_next(student_current);
    }
	return Success;
}

