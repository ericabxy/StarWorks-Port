#ifndef MODELS_H
#define MODELS_H

#include "types.h"

static const model cube = {
    .nVertex = 8, 
    .nEdges = 12,
	.vertices = (const point3D[]) {
        {50, 50, 50}, {-50, 50, 50}, 
        {50, -50, 50}, {50, 50, -50}, 
        {-50, -50, 50}, {50, -50, -50}, 
        {-50, 50, -50}, {-50, -50, -50}
    },

	.edges = (const edge[]) {
        {0,1}, {1,4},
        {4,2}, {2,0},

        {3,6}, {6,7},
        {7,5}, {5,3},

        {0,3}, {1,6}, 
        {2,5}, {4,7}   
    }
};

static const model arwing = { // i hate this shit
    .nVertex = 15,
    .nEdges = 20,
    .vertices = (const point3D[]) {
        // cockpit
        {0, 20, 0},
        {0, 0, -20},
        {-20, 0, 0}, // first connection
        {20, 0, 0}, // second connection
        {0, 0, 100},
        
        // diffuser 1
        {-20, -10, -20}, 
        {-20, -10, 20}, 
        {-20, 80, 0},

        // diffuser 2
        {20, -10, -20},
        {20, -10, 20},
        {20, 80, 0},

        // wing 1
        {-35, -5, 5},
        {-50, -30, -50},

        // wing 2
        {35, -5, 5},
        {50, -30, -50}
    },
    .edges = (const edge[]) {
        // cockpit
        {0, 1}, {0, 2}, 
        {0, 3}, {0, 4},
        {1, 2}, {1, 3},
        {2, 4}, {3, 4},

        // diffuser 1
        {5, 6}, {5, 7},
        {6, 7},

        // diffuser 2
        {8, 9}, {9, 10},
        {10, 8},

        // wing 1
        {2, 11}, {2, 12},
        {11, 12},

        //wing 2
        {3, 13}, {3, 14},
        {14, 13}
    }
};



#endif