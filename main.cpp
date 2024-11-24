#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

// Structure to hold planet properties
struct Planet {
    float size;              // Planet size
    float orbitRadius;       // Distance from sun
    float revolutionSpeed;   // Speed around sun
    float rotationSpeed;     // Speed of self-rotation
    float rotation;          // Current rotation angle
    float revolution;        // Current revolution angle
    float r, g, b;          // Planet color
    GLuint planetVBO;       // VBO for planet
    GLuint orbitVBO;        // VBO for orbit
};

// Function declarations
void keyboardMonitor(unsigned char key, int x, int y);
void specialKeysMonitor(int key, int x, int y);
void mouseMonitor(int button, int state, int x, int y);
void update();
void display();
void init();
void renderBitmapString(float x, float y, void *font, const char *string);
void reshape(int w, int h);
void generateStarPositions();
void initPlanet(Planet& planet, vector<GLfloat>& vertices);
void drawPlanet(const Planet& planet);

// Global variables
bool pauseAnimation = false;
float globalSpeed = 1.0f;
GLuint sunVBO, starsVBO;

// Star-related variables
const int NUM_STARS = 200;
vector<GLfloat> starVertices;

// Planet data
vector<Planet> planets;
vector<GLfloat> planetVertices;
const int NUM_PLANETS = 5; // Mercury, Venus, Earth, Mars, Jupiter

float randomFloat(float min, float max) {
    return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

void initPlanet(Planet& planet, vector<GLfloat>& vertices) {
    vertices.clear();
    const int segments = 36;
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        vertices.push_back(planet.size * cos(angle));
        vertices.push_back(planet.size * sin(angle));
    }

    // Create planet VBO
    glGenBuffers(1, &planet.planetVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planet.planetVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    // Create orbit VBO
    vector<GLfloat> orbitVertices;
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        orbitVertices.push_back(planet.orbitRadius * cos(angle));
        orbitVertices.push_back(planet.orbitRadius * sin(angle));
    }
    glGenBuffers(1, &planet.orbitVBO);
    glBindBuffer(GL_ARRAY_BUFFER, planet.orbitVBO);
    glBufferData(GL_ARRAY_BUFFER, orbitVertices.size() * sizeof(GLfloat), &orbitVertices[0], GL_STATIC_DRAW);
}

void generateStarPositions() {
    starVertices.clear();
    for (int i = 0; i < NUM_STARS; i++) {
        float x = randomFloat(-2.0f, 2.0f);
        float y = randomFloat(-2.0f, 2.0f);
        starVertices.push_back(x);
        starVertices.push_back(y);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
    glBufferData(GL_ARRAY_BUFFER, starVertices.size() * sizeof(GLfloat), &starVertices[0], GL_STATIC_DRAW);
}

void init() {
    srand(time(NULL));
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    
    // Initialize sun
    vector<GLfloat> sunVertices;
    for (int i = 0; i < 36; i++) {
        float angle = 2.0f * M_PI * i / 36;
        sunVertices.push_back(0.2f * cos(angle));
        sunVertices.push_back(0.2f * sin(angle));
    }
    
    glGenBuffers(1, &sunVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glBufferData(GL_ARRAY_BUFFER, sunVertices.size() * sizeof(GLfloat), &sunVertices[0], GL_STATIC_DRAW);

    // Initialize planets
    planets.resize(NUM_PLANETS);
    
    // Mercury
    planets[0].size = 0.04f;
    planets[0].orbitRadius = 0.3f;
    planets[0].revolutionSpeed = 4.1f;
    planets[0].rotationSpeed = 3.0f;
    planets[0].rotation = 0.0f;
    planets[0].revolution = 0.0f;
    planets[0].r = 0.7f;
    planets[0].g = 0.7f;
    planets[0].b = 0.7f;

    // Venus
    planets[1].size = 0.06f;
    planets[1].orbitRadius = 0.45f;
    planets[1].revolutionSpeed = 1.6f;
    planets[1].rotationSpeed = 2.0f;
    planets[1].rotation = 0.0f;
    planets[1].revolution = 0.0f;
    planets[1].r = 0.9f;
    planets[1].g = 0.7f;
    planets[1].b = 0.5f;

    // Earth
    planets[2].size = 0.08f;
    planets[2].orbitRadius = 0.65f;
    planets[2].revolutionSpeed = 1.0f;
    planets[2].rotationSpeed = 1.5f;
    planets[2].rotation = 0.0f;
    planets[2].revolution = 0.0f;
    planets[2].r = 0.2f;
    planets[2].g = 0.5f;
    planets[2].b = 1.0f;

    // Mars
    planets[3].size = 0.06f;
    planets[3].orbitRadius = 0.85f;
    planets[3].revolutionSpeed = 0.5f;
    planets[3].rotationSpeed = 1.2f;
    planets[3].rotation = 0.0f;
    planets[3].revolution = 0.0f;
    planets[3].r = 1.0f;
    planets[3].g = 0.3f;
    planets[3].b = 0.1f;

    // Jupiter
    planets[4].size = 0.15f;
    planets[4].orbitRadius = 1.2f;
    planets[4].revolutionSpeed = 0.08f;
    planets[4].rotationSpeed = 0.8f;
    planets[4].rotation = 0.0f;
    planets[4].revolution = 0.0f;
    planets[4].r = 0.8f;
    planets[4].g = 0.6f;
    planets[4].b = 0.4f;

    // Initialize each planet's VBOs
    for (int i = 0; i < NUM_PLANETS; i++) {
        initPlanet(planets[i], planetVertices);
    }

    // Initialize stars
    glGenBuffers(1, &starsVBO);
    generateStarPositions();
}

void drawPlanet(const Planet& planet) {
    // Draw orbit
    glColor3f(0.3f, 0.3f, 0.3f);
    glBindBuffer(GL_ARRAY_BUFFER, planet.orbitVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINE_LOOP, 0, 36);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Draw planet
    glPushMatrix();
    glRotatef(planet.revolution, 0, 0, 1);
    glTranslatef(planet.orbitRadius, 0, 0);
    glRotatef(planet.rotation, 0, 0, 1);
    
    glColor3f(planet.r, planet.g, planet.b);
    glBindBuffer(GL_ARRAY_BUFFER, planet.planetVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw stars
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_POINTS, 0, NUM_STARS);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Draw sun
    glColor3f(1.0f, 0.8f, 0.0f);
    glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Draw all planets
    for (int i = 0; i < planets.size(); ++i) {
        drawPlanet(planets[i]);
    }

    // Render text
    glColor3f(1.0f, 1.0f, 1.0f);
    renderBitmapString(-1.9f, 1.8f, GLUT_BITMAP_HELVETICA_12, "SPACE: pause");
    renderBitmapString(-1.9f, 1.7f, GLUT_BITMAP_HELVETICA_12, "UP/DOWN: speed");
    renderBitmapString(-1.9f, 1.6f, GLUT_BITMAP_HELVETICA_12, "CLICK: new stars");
    renderBitmapString(-1.9f, 1.5f, GLUT_BITMAP_HELVETICA_12, "ESC: exit");

    glutSwapBuffers();
}

void update() {
    if (!pauseAnimation) {
        for (int i = 0; i < planets.size(); ++i) {
            planets[i].revolution += planets[i].revolutionSpeed * globalSpeed;
            if (planets[i].revolution > 360.0f) planets[i].revolution -= 360.0f;
            
            planets[i].rotation += planets[i].rotationSpeed * globalSpeed;
            if (planets[i].rotation > 360.0f) planets[i].rotation -= 360.0f;
        }
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
            globalSpeed *= 1.1f;
            break;
        case GLUT_KEY_DOWN:
            globalSpeed *= 0.9f;
            break;
    }
}

void mouseMonitor(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        generateStarPositions();
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Solar System Simulation");
    
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        return -1;
    }
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardMonitor);
    glutSpecialFunc(specialKeysMonitor);
    glutMouseFunc(mouseMonitor);
    glutIdleFunc(update);
    
    glutMainLoop();
    return 0;
}
