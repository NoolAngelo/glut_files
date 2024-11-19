#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <cstdlib>

using namespace std;

void keyboardMonitor(unsigned char key, int x, int y);
void specialKeysMonitor(int key, int x, int y);
void update();
void initRandomColors();
void mouseMonitor(int button, int state, int x, int y);
void display();
void init();
void renderBitmapString(float x, float y, void *font, const char *string);
void reshape(int w, int h);

float px = 0.0f, py = 0.0f;
float StipplePosX = 0.0f;
float speed = 0.0001;
int SailColors[3][3];

GLuint sailVBO, boatVBO, lineVBO;

GLfloat sailVertices[] = {
    -0.2f, -0.1f,
     0.2f, -0.1f,
    -0.2f,  0.3f
};
GLfloat boatVertices[] = {
    -0.6f, -0.2f,
     0.6f, -0.2f,
     0.3f, -0.4f,
    -0.3f, -0.4f
};
GLfloat lineVertices[] = {
    -5.0f, -0.7f,  5.0f, -0.7f,
    -5.0f, -0.5f,  5.0f, -0.5f,
    -5.0f, -0.8f,  5.0f, -0.8f,
    -5.0f, -0.6f,  5.0f, -0.6f
};

void initRandomColors() {
    for (int i = 0; i < 3; ++i) {
        SailColors[i][0] = rand() % 256;
        SailColors[i][1] = rand() % 256;
        SailColors[i][2] = rand() % 256;
    }
}

void mouseMonitor(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        initRandomColors();
    }
}

void update() {
    StipplePosX += speed;
    if (StipplePosX > 1.0f || StipplePosX < -1.0f) {
        StipplePosX = -1.0f;
    }
    glutPostRedisplay();
}

void keyboardMonitor(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        exit(0);
    }
    glutPostRedisplay();
}

void specialKeysMonitor(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT: px -= 0.02f; break;
        case GLUT_KEY_RIGHT: px += 0.02f; break;
    }
    glutPostRedisplay();
}

void renderBitmapString(float x, float y, void *font, const char *string) {
    const char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the messages
    glColor3f(0.0f, 0.0f, 0.0f); // Set text color to black
    renderBitmapString(-0.95f, 0.9f, GLUT_BITMAP_HELVETICA_18, "Press ESC to exit");
    renderBitmapString(-0.95f, 0.85f, GLUT_BITMAP_HELVETICA_18, "Press left arrow to move left");
    renderBitmapString(-0.95f, 0.8f, GLUT_BITMAP_HELVETICA_18, "Press right arrow to move right");

    glPushMatrix();
    glTranslatef(px, py, 0);

    glColor3f(SailColors[0][0] / 255.0f, SailColors[0][1] / 255.0f, SailColors[0][2] / 255.0f);
    glBindBuffer(GL_ARRAY_BUFFER, sailVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glColor3f(0, 0, 0);
    glDrawArrays(GL_LINE_LOOP, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);

    glColor3f(0, 1.0f, 1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_POLYGON, 0, 4);
    glColor3f(0, 0, 0);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPopMatrix();

    glEnable(GL_LINE_STIPPLE);
    glLineWidth(1.0f);
    glPushMatrix();
    glTranslatef(StipplePosX, 0, 0);
    
    glColor3f(0, 0, 1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glLineStipple(3, 0x00FF);
    glDrawArrays(GL_LINES, 0, 4);
    
    glColor3f(1.0f, 0, 1.0f);
    glLineStipple(3, 0xFF00);
    glDrawArrays(GL_LINES, 4, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPopMatrix();
    glDisable(GL_LINE_STIPPLE);

    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        glOrtho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w, -1.0, 1.0);
    } else {
        glOrtho(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0, -1.0, 1.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glOrtho(-1, 1, -1, 1, -1, 1);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cerr << "Error initializing GLEW: " << glewGetErrorString(err) << endl;
        exit(1);
    }

    glGenBuffers(1, &sailVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sailVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sailVertices), sailVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &boatVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boatVertices), boatVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &lineVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("NoolAngelo_SA2");
    glutFullScreen();
    init();
    glutDisplayFunc(display);
    glutIdleFunc(update);
    glutKeyboardFunc(keyboardMonitor);
    glutSpecialFunc(specialKeysMonitor);
    glutMouseFunc(mouseMonitor);
    glutReshapeFunc(reshape); // Register the reshape callback function
    glutMainLoop();
    return 0;
}