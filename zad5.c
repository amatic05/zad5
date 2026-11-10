#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define ERROR_EXPRESSION -3

typedef struct Element {
    int data;
    struct Element* next;
} Element;

// Deklaracija funkcija
int push(Element** top, int value);
int pop(Element** top, int* value);
int calculate(char* expression);
int isOperator(char c);
int performOperation(int a, int b, char op);


//Početak glavne funkcije

int main() {
    FILE* fp = NULL;
	char expression[256];           // Buffer za pohranu izraza
    int result;                    

    fp = fopen("izraz.txt", "r");

    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return FILE_ERROR_OPEN;
    }

	// Čitanje izraza iz datoteke
    if (fgets(expression, sizeof(expression), fp) == NULL) {
        printf("Datoteka je prazna!\n");
        fclose(fp);
        return ERROR_EXPRESSION;
    }

    fclose(fp);

	// Izračunavanje izraza
    result = calculate(expression);

	// Provjera rezultata izračunavanja
    if (result < 0) {
        printf("Greska pri racunanju izraza!\n");
        return ERROR_EXPRESSION;
    }

	// Ispis rezultata
    printf("Rezultat izraza je: %d\n", result);

    return EXIT_SUCCESS;
}

//Početak funkcija:
// Funkcija za dodavanje elementa na vrh stoga

int push(Element** top, int value) {
    Element* newElement = NULL;

    newElement = (Element*)malloc(sizeof(Element));

    if (newElement == NULL) {
        return MALLOC_ERROR;
    }

    newElement->data = value;
    newElement->next = *top;
    *top = newElement;

    return EXIT_SUCCESS;
}


// Funkcija za uklanjanje elementa sa vrha stoga

int pop(Element** top, int* value) {
    Element* temp = NULL;

    if (*top == NULL) {
        return ERROR_EXPRESSION;
    }

    temp = *top;
    *value = temp->data;
    *top = temp->next;

    free(temp);

    return EXIT_SUCCESS;
}


// Funkcija za provjeru je li znak operator

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}


// Funkcija za izvođenje operacije

int performOperation(int a, int b, char op) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0) {
            return ERROR_EXPRESSION;
        }
        return a / b;
    default:
        return ERROR_EXPRESSION;
    }
}


// Funkcija za izračunavanje postfiksnog izraza

int calculate(char* expression) {
    Element* top = NULL;
    int i = 0;
    int a, b, result;

    while (expression[i] != '\0') {

        if (isspace(expression[i])) {
            i++;
        }
        else if (isdigit(expression[i])) {
            result = 0;

            while (isdigit(expression[i])) {
                result = result * 10 + (expression[i] - '0');
                i++;
            }

            if (push(&top, result) != EXIT_SUCCESS) {
                return MALLOC_ERROR;
            }
        }
        else if (isOperator(expression[i])) {

            if (pop(&top, &b) != EXIT_SUCCESS) {
                return ERROR_EXPRESSION;
            }

            if (pop(&top, &a) != EXIT_SUCCESS) {
                return ERROR_EXPRESSION;
            }

            result = performOperation(a, b, expression[i]);

            if (result == ERROR_EXPRESSION) {
                return ERROR_EXPRESSION;
            }

            if (push(&top, result) != EXIT_SUCCESS) {
                return MALLOC_ERROR;
            }

            i++;
        }
        else {
            return ERROR_EXPRESSION;
        }
    }

    if (top == NULL) {
        return ERROR_EXPRESSION;
    }

    if (top->next != NULL) {
        return ERROR_EXPRESSION;
    }

    pop(&top, &result);

    return result;
}
