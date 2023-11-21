#include <stdio.h>
#include <math.h>

typedef struct Point {
    float x;
    float y;
} Point;

float distance(Point A, Point B){
    return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

float area(Point A, Point B, Point C){
    float a = distance(A, B);
    float b = distance(B, C);
    float c = distance(C, A);
    
    return fabs(A.x * B.y + B.x * C.y + C.x * A.y - A.y * B.x - B.y * C.x - C.y * A.x) / 2;
}

int main(void){
    Point A = {2.5, 6};
    Point B = {1, 2.2};
    Point C = {10, 6};

    printf("Distance between A and B: %f\n", distance(A, B));

    printf("Area of triangle ABC: %f\n", area(A, B, C));
    
    return 0;

}