#include "graphics.h"
#include "constants.h"
#include "types.h"

//uint8_t framebuffer[HEIGHT][WIDTH];

static const uint32_t palette[5] = {
    0x000000, // black
    0xffffff, // white
    0x94c2ff, // light blue
    0x737573, // gray
    0xff0000 // red
};

void drawPixel(int x, int y, uint8_t color, uint32_t *framebuffer) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return;
	}
	framebuffer[y * WIDTH + x] = palette[color];
}

void drawLine(int x0, int y0, int x1, int y1, uint8_t color, uint32_t *framebuffer) {
	int deltaX = abs(x1 - x0);
	int deltaY = abs(y1 - y0);
	int parameter;
	int stepX = (x0 < x1) ? 1 : -1;
	int stepY = (y0 < y1) ? 1 : -1;
	int currentX = x0;
	int currentY = y0;
	drawPixel(currentX, currentY, color, framebuffer);

	// Bresenham's line algorithm

	if (deltaX >= deltaY) {
		parameter = 2 * deltaY - deltaX;
		for (int i = 0; i < deltaX; i++) {
			currentX+= stepX;
			if (parameter >= 0) {
				currentY+= stepY;
				parameter += (deltaY << 1) - (deltaX << 1);
			} else {
				parameter += (deltaY << 1);
			}
			drawPixel(currentX, currentY, color, framebuffer); 
		}
	} else {
		parameter = 2 * deltaX - deltaY;
		for (int i = 0; i < deltaY; i++) {
			currentY+= stepY;
			if (parameter >= 0) {
				currentX+= stepX;
				parameter += (deltaX << 1) - (deltaY << 1);
			} else {
				parameter += (deltaX << 1);
			}
			drawPixel(currentX, currentY, color, framebuffer); 
		}
	}
}

void drawModel(const model *m, camera *cam, point3D pos, uint8_t color, uint32_t *framebuffer) {
	//todo : use projectPoint()

	int finalCoordinates[m->nVertex][2]; // will contain the final coordinates to draw
	bool vertexValid[m->nVertex];
	
	for (int i = 0; i < m->nVertex; i++) {
		// from local position to global position
		int posX = pos.x + m->vertices[i].x - cam->position.x;
		int posY = pos.y + m->vertices[i].y - cam->position.y;
		int posZ = pos.z + m->vertices[i].z - cam->position.z;
		if (posZ < 1) {
			vertexValid[i] = false;
			continue;
		}
		
		vertexValid[i] = true;
		finalCoordinates[i][0] = (posX * FOCAL_DISTANCE) / posZ + CENTER_X;
		finalCoordinates[i][1] = - (posY * FOCAL_DISTANCE) / posZ + CENTER_Y;
	}

	for (int i = 0; i < m->nEdges; i++) {
		// get the indices of the two vertices connected by an edge
		int indexA = m->edges[i].indexA;
		int indexB = m->edges[i].indexB;

		if (vertexValid[indexA] && vertexValid[indexB]) {
			drawLine(finalCoordinates[indexA][0], finalCoordinates[indexA][1], finalCoordinates[indexB][0], finalCoordinates[indexB][1], color, framebuffer);
		}
	}
}

/* Unused: Framebuffer handled in (libretro.c).
void flushFrameBuffer() {
	eadk_color_t pushBuffer[WIDTH]; // line by line

	for (int offset = 0; offset < HEIGHT; offset++) {
		for (int x = 0; x < WIDTH; x++) { // converts our 8-bit colors to 16-bit (expected by the EADK)
			uint8_t colorIndex = framebuffer[offset][x];
			pushBuffer[x] = palette[colorIndex];
		}
		eadk_rect_t pushRect = {
			.x = (uint16_t) MARGIN,
			.y = (uint16_t) MARGIN + offset,
			.width = WIDTH,
			.height = 1
		};
		eadk_display_push_rect(pushRect, pushBuffer);
	}
	memset(framebuffer, 0, sizeof(framebuffer));
}*/

void drawGround(camera *cam, uint32_t *framebuffer) {
	// the entiere function is just a screenSize = realSize/distance
	// it just draws lines on the ground

	for (int lineNumber = 0; lineNumber < NUMBER_LINES; lineNumber++) {

		int currentZ = cam->position.z + lineNumber * LINE_SPACING; // relative z
		currentZ -= currentZ % LINE_SPACING; 

		point3D leftDot = {-GROUND_WIDTH, GROUND_Y, currentZ};
		point3D rightDot = {GROUND_WIDTH, GROUND_Y, currentZ};
		
		smallPoint2D leftDotScreen = projectPoint(leftDot, cam);
		smallPoint2D rightDotScreen = projectPoint(rightDot, cam);

		if ((leftDotScreen.x != 1 || leftDotScreen.y != 1) && (rightDotScreen.x != 1 || rightDotScreen.y != 1)) {
			drawLine(leftDotScreen.x, leftDotScreen.y, rightDotScreen.x, rightDotScreen.y, 3, framebuffer);
		}
	}
}

smallPoint2D projectPoint(point3D toProject, camera *cam) {
	

	int distanceX = toProject.x - cam->position.x;
	int distanceY = toProject.y - cam->position.y;
	int distanceZ = toProject.z - cam->position.z;
	
	if (distanceZ < 1) {
		return (smallPoint2D){-1, -1};
	}

	return (smallPoint2D) {
		(distanceX*FOCAL_DISTANCE) / distanceZ + CENTER_X,
		-(distanceY*FOCAL_DISTANCE) / distanceZ + CENTER_Y
	};
}
