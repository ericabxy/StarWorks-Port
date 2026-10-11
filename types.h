#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	int16_t x, y;
} smallPoint2D;

typedef struct {
	int x, y;
} point2D;

typedef struct {
	int x, y, z;
} point3D;

typedef struct {
	int16_t x, y, z;
} smallPoint3D;

typedef struct {
	int indexA, indexB;
} edge;

typedef struct {
	const uint16_t nVertex, nEdges;
	const point3D *vertices;
	const edge *edges;
} model;

typedef struct {
    const model *shipModel;
    smallPoint3D rotation;
    point3D position;
} ship;

typedef struct {
    point3D position;
    smallPoint3D rotation;
} camera;

typedef struct {
    point3D position;
    smallPoint3D direction;
    bool owner; // 0 -> player, 1 -> enemy
    bool active;
} bullet;


#endif
