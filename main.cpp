#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

// Function declarations
void keyboardMonitor(unsigned char key, int x, int y);
void specialKeysMonitor(int key, int x, int y);
void update();
void display();
void init();
void renderBitmapString(float x, float y, void *font, const char *string);
void reshape(int w, int h);

// Global variables
float rotation = 0.0f;
float speed = 0.5f;
bool pauseAnimation = false;
GLuint sunVBO, earthVBO, orbitVBO;

// Vertex data
GLfloat sunVertices[72];    // Circle vertices for sun
GLfloat earthVertices[72];  // Circle vertices for earth
GLfloat orbitVertices[72];  // Circle vertices for orbit

void initCircleVertices() {
    // Generate circle vertices
    for (int i = 0; i < 36; i++) {
        float angle = 2.0f * M_PI * i / 36;
        // Sun vertices (larger circle)
        sunVertices[i*2] = 0.2f * cos(angle);
        sunVertices[i*2+1] = 0.2f * sin(angle);
        // Earth vertices (smaller circle)
        earthVertices[i*2] = 0.1f * cos(angle);
        earthVertices[i*2+1] = 0.1f * sin(angle);
        // Orbit vertices
        orbitVertices[i*2] = 0.5f * cos(angle);
        orbitVertices[i*2+1] = 0.5f * sin(angle);
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    
    initCircleVertices();

    // Create and bind VBOs
    glGenBuffers(1, &sunVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sunVertices), sunVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &earthVBO);
    glBindBuffer(GL_ARRAY_BUFFER, earthVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(earthVertices), earthVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &orbitVBO);
    glBindBuffer(GL_ARRAY_BUFFER, orbitVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(orbitVertices), orbitVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Render text
    glColor3f(1.0f, 1.0f, 1.0f);
    renderBitmapString(-0.9f, 0.9f, GLUT_BITMAP_HELVETICA_18, "Press SPACE to pause");
    renderBitmapString(-0.9f, 0.8f, GLUT_BITMAP_HELVETICA_18, "Press ESC to exit");

    // Draw sun
    glColor3f(1.0f, 0.8f, 0.0f);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Draw orbit
    glColor3f(0.5f, 0.5f, 0.5f);
    glBindBuffer(GL_ARRAY_BUFFER, orbitVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINE_LOOP, 0, 36);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Draw earth
    glPushMatrix();
    glRotatef(rotation, 0, 0, 1);
    glTranslatef(0.5f, 0, 0);
    glColor3f(0.2f, 0.5f, 1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, earthVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();

    glutSwapBuffers();
}

void update() {
    if (!pauseAnimation) {
        rotation += speed;
        if (rotation > 360.0f) rotation -= 360.0f;
    }
    glutPostRedisplay();
}

void keyboardMonitor(unsigned char key, int x, int y) {
    switch (key) {
        case 27:    // ESC
            exit(0);
            break;
        case ' ':   // Space
            pauseAnimation = !pauseAnimation;
            break;
    }
}

void specialKeysMonitor(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            speed *= 1.1f;
            break;
        case GLUT_KEY_DOWN:
            speed *= 0.9f;
            break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Maintain aspect ratio and view all planets
    float aspect = (float)w / (float)h;
    if (w <= h) {
        glOrtho(-2.0, 2.0, -2.0/aspect, 2.0/aspect, -1.0, 1.0);
    } else {
        glOrtho(-2.0*aspect, 2.0*aspect, -2.0, 2.0, -1.0, 1.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void renderBitmapString(float x, float y, void *font, const char *string) {
    glRasterPos2f(x, y);
    while (*string) {
        glutBitmapCharacter(font, *string);
        string++;
    }
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Simple Solar System");
    
    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        return -1;
    }
    
    // Setup
    init();
    
    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardMonitor);
    glutSpecialFunc(specialKeysMonitor);
    glutIdleFunc(update);
    
    // Start main loop
    glutMainLoop();
    return 0;
}
