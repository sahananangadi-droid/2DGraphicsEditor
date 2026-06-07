/*
 * 2D Graphics Editor
 * Author: Sahana
 * Description: A menu-driven 2D graphics editor in C
 *              Uses a 2D character array with * and _
 *              Supports: Rectangle, Line, Circle, Triangle
 *              Features: Add, Delete, Clear, Display objects
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 25
#define COLS 60

char canvas[ROWS][COLS];

void initCanvas()
{
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            canvas[i][j] = '_';
}

void displayCanvas()
{
    printf("\n");
    printf("   ");
    for (int j = 0; j < COLS; j += 5)
        printf("%-5d", j);
    printf("\n");
    for (int i = 0; i < ROWS; i++)
    {
        printf("%2d ", i);
        for (int j = 0; j < COLS; j++)
            printf("%c", canvas[i][j]);
        printf("\n");
    }
    printf("\n-- Canvas displayed. Choose next option --\n");
}

int inBounds(int x, int y)
{
    return x >= 0 && x < COLS && y >= 0 && y < ROWS;
}

void plot(int x, int y)
{
    if (inBounds(x, y))
        canvas[y][x] = '*';
}

void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    while (1)
    {
        plot(x1, y1);
        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2)
{
    if (!inBounds(x1, y1) || !inBounds(x2, y2))
    {
        printf("Error: out of bounds! Max x=%d, Max y=%d\n", COLS - 1, ROWS - 1);
        return;
    }
    drawLine(x1, y1, x2, y1);
    drawLine(x1, y2, x2, y2);
    drawLine(x1, y1, x1, y2);
    drawLine(x2, y1, x2, y2);
}

void drawCircle(int xc, int yc, int r)
{
    if (!inBounds(xc - r, yc - r) || !inBounds(xc + r, yc + r))
    {
        printf("Error: circle out of bounds! Max x=%d, Max y=%d\n", COLS - 1, ROWS - 1);
        return;
    }
    int x = 0, y = r, d = 3 - 2 * r;
    while (y >= x)
    {
        plot(xc + x, yc + y);
        plot(xc - x, yc + y);
        plot(xc + x, yc - y);
        plot(xc - x, yc - y);
        plot(xc + y, yc + x);
        plot(xc - y, yc + x);
        plot(xc + y, yc - x);
        plot(xc - y, yc - x);
        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

#define MAX_OBJECTS 20
typedef struct
{
    int type, x1, y1, x2, y2, x3, y3, r;
} Object;
Object objects[MAX_OBJECTS];
int objCount = 0;

void redrawAll()
{
    initCanvas();
    for (int i = 0; i < objCount; i++)
    {
        Object o = objects[i];
        if (o.type == 1)
            drawRectangle(o.x1, o.y1, o.x2, o.y2);
        else if (o.type == 2)
            drawLine(o.x1, o.y1, o.x2, o.y2);
        else if (o.type == 3)
            drawCircle(o.x1, o.y1, o.r);
        else if (o.type == 4)
            drawTriangle(o.x1, o.y1, o.x2, o.y2, o.x3, o.y3);
    }
}

void listObjects()
{
    if (objCount == 0)
    {
        printf("No objects on canvas.\n");
        return;
    }
    printf("\n--- Objects on Canvas ---\n");
    for (int i = 0; i < objCount; i++)
    {
        Object o = objects[i];
        printf("[%d] ", i + 1);
        if (o.type == 1)
            printf("Rectangle (%d,%d) to (%d,%d)\n", o.x1, o.y1, o.x2, o.y2);
        else if (o.type == 2)
            printf("Line      (%d,%d) to (%d,%d)\n", o.x1, o.y1, o.x2, o.y2);
        else if (o.type == 3)
            printf("Circle    center(%d,%d) r=%d\n", o.x1, o.y1, o.r);
        else if (o.type == 4)
            printf("Triangle  (%d,%d),(%d,%d),(%d,%d)\n", o.x1, o.y1, o.x2, o.y2, o.x3, o.y3);
    }
}

void deleteObject()
{
    listObjects();
    if (objCount == 0)
        return;
    int idx;
    printf("Enter object number to delete (1-%d): ", objCount);
    scanf("%d", &idx);
    if (idx < 1 || idx > objCount)
    {
        printf("Invalid!\n");
        return;
    }
    for (int i = idx - 1; i < objCount - 1; i++)
        objects[i] = objects[i + 1];
    objCount--;
    redrawAll();
    printf("Object deleted!\n");
}

int main()
{
    initCanvas();
    int choice;
    while (1)
    {
        printf("\n=============================\n");
        printf("     2D Graphics Editor\n");
        printf("=============================\n");
        printf(" Valid x: 0-%d, Valid y: 0-%d\n", COLS - 1, ROWS - 1);
        printf("-----------------------------\n");
        printf(" 1. Draw Rectangle\n");
        printf(" 2. Draw Line\n");
        printf(" 3. Draw Circle\n");
        printf(" 4. Draw Triangle\n");
        printf(" 5. Delete an Object\n");
        printf(" 6. Clear Canvas\n");
        printf(" 7. Display Canvas\n");
        printf(" 8. List Objects\n");
        printf(" 9. Exit\n");
        printf("=============================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            int x1, y1, x2, y2;
            printf("Enter top-left (x1 y1): ");
            scanf("%d %d", &x1, &y1);
            printf("Enter bottom-right (x2 y2): ");
            scanf("%d %d", &x2, &y2);
            if (objCount < MAX_OBJECTS)
            {
                objects[objCount++] = (Object){1, x1, y1, x2, y2, 0, 0, 0};
                drawRectangle(x1, y1, x2, y2);
                displayCanvas();
            }
        }
        else if (choice == 2)
        {
            int x1, y1, x2, y2;
            printf("Enter start (x1 y1): ");
            scanf("%d %d", &x1, &y1);
            printf("Enter end (x2 y2): ");
            scanf("%d %d", &x2, &y2);
            if (objCount < MAX_OBJECTS)
            {
                objects[objCount++] = (Object){2, x1, y1, x2, y2, 0, 0, 0};
                drawLine(x1, y1, x2, y2);
                displayCanvas();
            }
        }
        else if (choice == 3)
        {
            int xc, yc, r;
            printf("Enter center (xc yc): ");
            scanf("%d %d", &xc, &yc);
            printf("Enter radius: ");
            scanf("%d", &r);
            if (objCount < MAX_OBJECTS)
            {
                objects[objCount++] = (Object){3, xc, yc, 0, 0, 0, 0, r};
                drawCircle(xc, yc, r);
                displayCanvas();
            }
        }
        else if (choice == 4)
        {
            int x1, y1, x2, y2, x3, y3;
            printf("Enter point 1 (x1 y1): ");
            scanf("%d %d", &x1, &y1);
            printf("Enter point 2 (x2 y2): ");
            scanf("%d %d", &x2, &y2);
            printf("Enter point 3 (x3 y3): ");
            scanf("%d %d", &x3, &y3);
            if (objCount < MAX_OBJECTS)
            {
                objects[objCount++] = (Object){4, x1, y1, x2, y2, x3, y3, 0};
                drawTriangle(x1, y1, x2, y2, x3, y3);
                displayCanvas();
            }
        }
        else if (choice == 5)
        {
            deleteObject();
            displayCanvas();
        }
        else if (choice == 6)
        {
            objCount = 0;
            initCanvas();
            printf("Canvas cleared!\n");
            displayCanvas();
        }
        else if (choice == 7)
        {
            displayCanvas();
        }
        else if (choice == 8)
        {
            listObjects();
        }
        else if (choice == 9)
        {
            printf("Goodbye!\n");
            break;
        }
        else
        {
            printf("Invalid choice!\n");
        }
    }
    return 0;
}