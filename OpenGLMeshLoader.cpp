#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <math.h>
#include <string>
#include <iostream>
#include <irrKlang.h>





#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Window dimensions
int WIDTH = 1280;
int HEIGHT = 720;
irrklang::ISoundEngine* engine2 = irrklang::createIrrKlangDevice();
irrklang::ISoundEngine* engine3 = irrklang::createIrrKlangDevice();



GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};





float charPosX = 10.0f;
float charPosY = 0.0f;
float charPosZ = 0.0f;
float charSpeed = 0.5f;
float jumpHeight = 10.0f; // Height of the jump
bool isJumping = false; // Flag to check if the character
int animationFrame = 0;
float animationTime = 0.0f;
float frameDuration = 0.1f; // Duration of each frame in 
float animationSpeed = 0.5f; // Speed of the animation
bool isMoving = false; // Tracks if the character is currently moving
int timerSeconds = 60; // 60 seconds timer
int currentWeapon = 0; // 0: Gun, 1: Bow, 2: Knife
float playerRotationAngle = 360.0f;
float timeOfDay = 0.0f;
bool isOver = false;
int score = 0;




// Camera Settings
float cameraAngleX = 0.0f;  // Horizontal rotation angle
float cameraAngleY = 0.0f;  // Vertical rotation angle
float cameraDistance = 20.0f; // Distance from the target in third-person view
int cameraMode = 1;          // 0: First-person, 1: Third-person
Vector Eye;                  // Camera position
Vector At;                   // Look-at position
Vector Up(0, 1, 0);          // Up direction for the camera

int cameraZoom = 0;

// Model Variables
Model_3DS model_gun1; //
Model_3DS model_char; //
Model_3DS model_bow; //
Model_3DS model_knife; //
Model_3DS model_aid; //collision part na2es
Model_3DS model_barrel; //collision part na2es
Model_3DS model_bush;//collision part na2es
Model_3DS model_coin; //collision part na2es
Model_3DS model_house;
Model_3DS model_syringe;//mafeesh 8erha hia el collisions na2sa brdo henna
Model_3DS model_tree; //w henna
Model_3DS model_truck;//w akeed henna brdo :)
Model_3DS model_wall;
Model_3DS model_zombie; //
Model_3DS model_rhino; //
Model_3DS model_sun; //



// Textures
GLTexture tex_ground;


//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	 //Enable Light Source number 0
	 //OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT1);
	GLfloat ambient1[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);

	GLfloat diffuse1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);

	GLfloat specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);

	GLfloat light_position1[] = { charPosX, charPosY + 5.0f, charPosZ, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	GLfloat light_direction[] = { -1.0f, -1.0f, -1.0f };
	GLfloat light_angle = 15.0f;
	GLfloat light_exponent = 10.0f;

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light_angle);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, light_exponent);
}

void UpdateLightPosition(float deltaTime) {
	timeOfDay += deltaTime / timerSeconds;
	if (timeOfDay > 1.0f) timeOfDay -= 1.0f;

	// Calculate light position based on time of day
	float angle = timeOfDay * 2.0f * 3.14159f; // Map to 0 - 2π for a full circle
	float radius = 15.0f; // Radius of the sun/moon orbit
	float x = radius * std::cos(angle);
	float y = radius * std::sin(angle); // Vertical movement
	float z = 0.0f; // Keep z constant for simplicity

	GLfloat light_position[] = { x, y, z, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void UpdateSpotlight() {
	//if (isIncreasingSun) {
	//	sunIntensity += sunTransitionSpeed;
	//	if (sunIntensity >= 1.0f) {
	//		sunIntensity = 1.0f; // Maximum brightness
	//		isIncreasingSun = false; // Start dimming
	//	}
	//}
	//else {
	//	sunIntensity -= sunTransitionSpeed;
	//	if (sunIntensity <= 0.1f) {
	//		sunIntensity = 0.1f; // Minimum brightness
	//		isIncreasingSun = true; // Start brightening
	//	}
	//}

	// Spotlight position (e.g., like a streetlamp)
	GLfloat position[] = { charPosX, charPosY + 30.0f, charPosZ, 1.0f }; // Positional light source
	GLfloat direction[] = { 0.0, -1.0f, 0.0f }; // Direction pointing downward

	// Spotlight properties
	GLfloat diffuseLight[] = { 1, 1, 1, 1.0f }; // Brightness of the spotlight
	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Low ambient component for soft illumination

	// Set light properties
	glLightfv(GL_LIGHT4, GL_POSITION, position); // Position of the spotlight
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuseLight); // Diffuse light color
	glLightfv(GL_LIGHT4, GL_AMBIENT, ambientLight); // Ambient light color
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, direction); // Spotlight direction

	// Spotlight-specific settings
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 50.0f); // Angle of the spotlight cone (in degrees)
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 2.0f); // Intensity distribution (higher = sharper falloff)
	glEnable(GL_LIGHT4);
}

void UpdateSun() {
	//if (isIncreasingSun) {
	//	sunIntensity += sunTransitionSpeed;
	//	if (sunIntensity >= 1.0f) {
	//		sunIntensity = 1.0f; // Maximum brightness
	//		isIncreasingSun = false; // Start dimming
	//	}
	//}
	//else {
	//	sunIntensity -= sunTransitionSpeed;
	//	if (sunIntensity <= 0.1f) {
	//		sunIntensity = 0.1f; // Minimum brightness
	//		isIncreasingSun = true; // Start brightening
	//	}
	//}

	// Spotlight position (e.g., like a streetlamp)
	GLfloat position[] = { 0, 50.0, 0, 1.0f }; // Positional light source
	GLfloat direction[] = { 0.0, -1.0f, 0.0f }; // Direction pointing downward

	// Spotlight properties
	GLfloat diffuseLight[] = { 1, 1, 1, 1.0f }; // Brightness of the spotlight
	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Low ambient component for soft illumination

	// Set light properties
	glLightfv(GL_LIGHT5, GL_POSITION, position); // Position of the spotlight
	glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuseLight); // Diffuse light color
	glLightfv(GL_LIGHT5, GL_AMBIENT, ambientLight); // Ambient light color
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, direction); // Spotlight direction

	// Spotlight-specific settings
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 100.0f); // Angle of the spotlight cone (in degrees)
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 2.0f); // Intensity distribution (higher = sharper falloff)
	glEnable(GL_LIGHT5);
}


void DrawSunOrMoon(float timeOfDay) {
	float angle = timeOfDay * 2.0f * 3.14159f;
	float radius = 15.0f;
	float x = radius * std::cos(angle);
	float y = radius * std::sin(angle);
	float z = 0.0f;

	glPushMatrix();
	glTranslatef(x, y+50.0f, z); // Move sphere to light position

	// Change color based on timeOfDay

	glColor3f(1.0f, 1.0f, 0.5f);
	
	

	// Render sphere (sun or moon)
	glutSolidSphere(1.0f, 30, 30); // Radius = 1.0, 30 slices, 30 stacks
	glPopMatrix();
}

void UpdateLightingColors(float timeOfDay) {
	GLfloat ambient[4], diffuse[4], specular[4];

	if (timeOfDay <= 0.5f) {
		// Daytime (Sun)
		float intensity = 0.5f + 0.5f * std::cos(timeOfDay * 2.0f * 3.14159f); // Peak at noon
		ambient[0] = ambient[1] = ambient[2] = 0.2f * intensity;
		diffuse[0] = 1.0f * intensity;
		diffuse[1] = 0.9f * intensity;
		diffuse[2] = 0.7f * intensity;
		specular[0] = specular[1] = specular[2] = 1.0f * intensity;
	}
	else {
		// Nighttime (Moon)
		float intensity = 0.2f + 0.3f * std::cos((timeOfDay - 0.5f) * 2.0f * 3.14159f); // Dim at midnight
		ambient[0] = ambient[1] = ambient[2] = 0.1f * intensity;
		diffuse[0] = diffuse[1] = 0.8f * intensity;
		diffuse[2] = 1.0f * intensity;
		specular[0] = specular[1] = specular[2] = 0.8f * intensity;
	}

	ambient[3] = diffuse[3] = specular[3] = 1.0f; // Alpha

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}


void InitHeadLamp()
{
	glEnable(GL_LIGHT1); // Enable Light Source 1 for the headlamp

	// Set ambient light (dim)
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

	// Set diffuse light (bright cone)
	GLfloat diffuse[] = { 0.9f, 0.9f, 0.7f, 1.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);

	// Set specular light (sharp highlights)
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);

	// Configure spotlight properties
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f); // Spotlight angle
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0f); // Spotlight intensity
}

#define PI 3.14159265358979323846

// Convert degrees to radians
float DegreesToRadians(float degrees)
{
	return degrees * (PI / 180.0f);
}

void UpdateHeadLamp()
{
	// Calculate headlamp position (relative to character's head)
	GLfloat headlampPosition[] = { charPosX, charPosY + 5.0f, charPosZ, 1.0f }; // Adjust offset for head height
	glLightfv(GL_LIGHT1, GL_POSITION, headlampPosition);

	// Calculate headlamp direction (character is facing)
	float angleRadians = DegreesToRadians(playerRotationAngle);
	GLfloat headlampDirection[] = {
		sin(angleRadians), // X direction
		-0.1f,             // Slightly downward
		-cos(angleRadians) // Z direction
	};
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, headlampDirection);
}


void DrawCylinder(float baseRadius, float topRadius, float height, int slices)
{
	float angleStep = 2.0f * PI / slices;

	// Draw the side of the cylinder
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= slices; ++i)
	{
		float angle = i * angleStep;
		float x = cos(angle);
		float z = sin(angle);

		// Bottom vertex
		glNormal3f(x, 0.0f, z); // Normal for lighting
		glVertex3f(baseRadius * x, 0.0f, baseRadius * z);

		// Top vertex
		glNormal3f(x, 0.0f, z); // Normal for lighting
		glVertex3f(topRadius * x, height, topRadius * z);
	}
	glEnd();

	// Draw the bottom cap
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0f, -1.0f, 0.0f); // Normal pointing down
	glVertex3f(0.0f, 0.0f, 0.0f); // Center of the bottom cap
	for (int i = 0; i <= slices; ++i)
	{
		float angle = i * angleStep;
		float x = cos(angle);
		float z = sin(angle);
		glVertex3f(baseRadius * x, 0.0f, baseRadius * z);
	}
	glEnd();

	// Draw the top cap
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0f, 1.0f, 0.0f); // Normal pointing up
	glVertex3f(0.0f, height, 0.0f); // Center of the top cap
	for (int i = 0; i <= slices; ++i)
	{
		float angle = i * angleStep;
		float x = cos(angle);
		float z = sin(angle);
		glVertex3f(topRadius * x, height, topRadius * z);
	}
	glEnd();
}


void DrawHeadLamp()
{
	glPushMatrix();
	// Position the headlamp relative to the character's head
	glTranslatef(charPosX, charPosY + 5.0f, charPosZ); // Adjust for head height
	glRotatef(playerRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate with the character
	glColor3f(0.2f, 0.2f, 0.2f); // Dark gray color for the headlamp body
	DrawCylinder(0.5f, 1.0f, 16, 16); // Cylinder as the lamp
	glPopMatrix();
}





//=======================================================================
// Material Configuration Function
//======================================================================


void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}



void setCamera() {
	if (cameraMode == 0) { // First-person
		Eye.x = charPosX;
		Eye.y = charPosY + 1.8f;
		Eye.z = charPosZ;

		At.x = Eye.x + sin(cameraAngleX * M_PI / 180.0);
		At.y = Eye.y;
		At.z = Eye.z - cos(cameraAngleX * M_PI / 180.0);
	}
	else if (cameraMode == 1) { // Third-person
		Eye.x = charPosX - cameraDistance * sin(cameraAngleX * M_PI / 180.0);
		Eye.y = charPosY + 9.0f;
		Eye.z = charPosZ + cameraDistance * cos(cameraAngleX * M_PI / 180.0);

		At.x = charPosX;
		At.y = charPosY + 1.0f;
		At.z = charPosZ;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}


//=======================================================================
// OpenGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*//

	//InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

void RenderTimer()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // Save the current projection matrix
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, 0, HEIGHT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // Save the current modelview matrix
	glLoadIdentity();

	glDisable(GL_LIGHTING); // Disable lighting for text rendering
	glColor3f(0.0, 0.0, 0.0);

	glRasterPos2f(10, HEIGHT - 20);

	std::string timerText = "Time: " + std::to_string(timerSeconds) + "s";
	for (char c : timerText)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}

	glPopMatrix(); // Restore the previous modelview matrix

	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); // Restore the previous projection matrix

	glMatrixMode(GL_MODELVIEW); // Ensure the modelview matrix mode is active
	glEnable(GL_LIGHTING); // Re-enable lighting
}

void RenderScore()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // Save the current projection matrix
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, 0, HEIGHT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // Save the current modelview matrix
	glLoadIdentity();

	glDisable(GL_LIGHTING); // Disable lighting for text rendering
	glColor3f(0.0, 0.0, 0.0);

	glRasterPos2f(10, HEIGHT - 50); // Adjust the position for score display

	std::string scoreText = "Score: " + std::to_string(score);
	for (char c : scoreText)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}

	glPopMatrix(); // Restore the previous modelview matrix

	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); // Restore the previous projection matrix

	glMatrixMode(GL_MODELVIEW); // Ensure the modelview matrix mode is active
	glEnable(GL_LIGHTING); // Re-enable lighting
}



void TimerCallback(int value)
{
	if (timerSeconds > 0)
	{
		timerSeconds--;
		glutPostRedisplay(); // Request a redraw
		glutTimerFunc(1000, TimerCallback, 0); // Call this function again in 1 second
	}
}


void updateAnimation(float deltaTime)
{
	if (isMoving)
	{
		animationTime += deltaTime;

		// Update animation frame
		if (animationTime >= frameDuration)
		{
			animationFrame = (animationFrame + 1) % 4; // Assuming 4 animation frames
			animationTime = 0.0f;
		}
	}
	else
	{
		animationFrame = 0; // Reset to the idle frame if not moving
	}
}



//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-200, 0, -200);
	glTexCoord2f(50, 0);
	glVertex3f(200, 0, -200);
	glTexCoord2f(50, 50);
	glVertex3f(200, 0, 200);
	glTexCoord2f(0, 50);
	glVertex3f(-200, 0, 200);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void drawArrow() {
	glPushMatrix();
	glTranslatef(0.01f, 1.5f, 1.0f); // Position the arrow
	glScaled(0.7, 0.7, 0.5);
	glRotatef(90, 0, 1, 0);
	glRotated(90, 0, 0, 1);
	glRotated(90, 1.0, 0, 0);

	// Shaft of the arrow - Cylinder
	glColor3f(0.8f, 0.8f, 0.8f); // Light gray color
	GLUquadric* quad = gluNewQuadric();
	gluCylinder(quad, 0.05, 0.05, 2.0, 20, 5); // Arrow shaft

	// Arrowhead - Cone
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 2.0f); // Position at end of shaft
	glColor3f(1.0f, 0.0f, 0.0f); // Red color for the arrowhead
	glutSolidCone(0.1, 0.3, 20, 10); // Arrowhead
	glPopMatrix();

	// Fletchings (feathers) at the back of the arrow
	glColor3f(0.7f, 0.7f, 0.7f); // Gray color for fletchings

	// Right fletching
	glPushMatrix();
	glTranslatef(0.1f, 0.0f, -0.2f); // Position at back of the shaft
	glRotatef(30, 0.0f, 1.0f, 0.0f); // Rotate fletching
	glutSolidCone(0.05, 0.2, 10, 5); // Right fletching
	glPopMatrix();

	// Left fletching
	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, -0.2f); // Position at back of the shaft
	glRotatef(-30, 0.0f, 1.0f, 0.0f); // Rotate fletching
	glutSolidCone(0.05, 0.2, 10, 5); // Left fletching
	glPopMatrix();

	// Top fletching
	glPushMatrix();
	glTranslatef(0.0f, 0.1f, -0.2f); // Position at back of the shaft
	glRotatef(90, 1.0f, 0.0f, 0.0f); // Rotate top fletching
	glutSolidCone(0.05, 0.2, 10, 5); // Top fletching
	glPopMatrix();

	gluDeleteQuadric(quad);
	glPopMatrix();
}

class Projectile {
public:
	Vector position;
	Vector velocity;
	float size;
	bool isActive;
	int type; // 0 for bullet, 1 for arrow

	Projectile() : position(10.0f, 10.0f, 10.0f), velocity(0.0f, 0.0f, 0.0f), size(0.1f), isActive(false), type(0) {}

	// Constructor to initialize the projectile with proper position and velocity
	Projectile(float _x, float _y, float _z, Vector _velocity, int _type)
		: position(_x, _y, _z), velocity(_velocity), size(0.1f), isActive(true), type(_type) {}

	// Update the position based on velocity
	void update(float deltaTime) {
		if (isActive) {
			position.x += velocity.x * deltaTime;
			position.y += velocity.y * deltaTime;
			position.z += velocity.z * deltaTime;

			// Deactivate the projectile when it goes off-screen (for example)
			if (abs(position.x) > 100 || abs(position.z) > 100) {
				isActive = false;
			}
		}
	}

	// Render the projectile
	void render() {
		if (isActive) {
			glPushMatrix();

			if (playerRotationAngle == 360.0f || playerRotationAngle == 0.0f) {
				glTranslatef(position.x + 2.0f, position.y + 3.0f, position.z - 2.3f);
			}

			else if (playerRotationAngle == 270.0f) {
				glTranslatef(position.x + 2.0f, position.y + 3.0f, position.z + 2.0f);
			}

			if (playerRotationAngle == 180.0f) {
				glTranslatef(position.x - 2.0f, position.y + 3.0f, position.z + 0.5f);
			}

			if (playerRotationAngle == 90.0f) {
				glTranslatef(position.x + 5.0f, position.y + 3.0f, position.z - 2.3f);
			}

			if (type == 0) {  // Bullet
				glColor3f(1.0f, 0.0f, 0.0f);  // Red color for bullet
				glutSolidSphere(size, 10, 10);  // Draw bullet
			}
			else if (type == 1) {  // Arrow
				glColor3f(0.8f, 0.8f, 0.0f);  // Yellow color for arrow
				drawArrow();  // Assuming you have the drawArrow() function defined
			}
			glPopMatrix();
		}
	}
};






// Maximum number of projectiles
const int MAX_PROJECTILES = 100;
Projectile projectiles[MAX_PROJECTILES];
int numProjectiles = 0; // Number of active projectiles

class Rhino {
public:
	Vector position;
	bool isAlive;
	float size;
	Rhino() : position(0, 0, 0), isAlive(true), size(10.0f) {}
	Rhino(float x, float y, float z, float _size) : position(x, y, z), isAlive(true), size(_size) {}

	void render() {
		if (isAlive) {
			glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			model_rhino.Draw();// Draw a simple sphere as a placeholder
			glPopMatrix();
		}
	}

	bool checkCollision(const Projectile& p) {
		float dist = sqrt(pow(p.position.x - position.x, 2) +
			pow(p.position.y - position.y, 2) +
			pow(p.position.z - position.z, 2));
		return dist < size + p.size;
	}

	bool checkCharCollisionUP() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ - charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size + 5.0f;
	}

	bool checkCharCollisionDOWN() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ + charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size + 5.0f;
	}

	bool checkCharCollisionRIGHT() {
		float dist = sqrt(pow(charPosX + charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size + 5.0f;
	}

	bool checkCharCollisionLEFT() {
		float dist = sqrt(pow(charPosX - charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size + 5.0f;
	}

	void die() {
		isAlive = false;
	}
};

// Define Zombie Class
class Zombie {
public:
	Vector position;
	bool isAlive;
	float size;
	float renderScale;

	Zombie() : position(0, 0, 0), isAlive(false), size(10.0f), renderScale(1.0f) {}

	Zombie(float x, float y, float z, float _size, float _renderScale) : position(x, y, z), isAlive(true), size(_size), renderScale(_renderScale) {}

	void render() {
		if (isAlive) {
			glPushMatrix();
			glTranslatef(position.x, position.y, position.z + 1.5f);
			glScalef(renderScale, renderScale, renderScale); // Scale for rendering
			model_zombie.Draw(); // Placeholder for zombie
			glPopMatrix();
		}
	}

	bool checkCollision(const Projectile& p) {
		float dist = sqrt(pow(p.position.x - position.x, 2) +
			pow(p.position.y - position.y, 2) +
			pow(p.position.z - position.z, 2));
		return dist < size + p.size;
	}

	bool checkCharCollisionUP() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ - charSpeed - position.z, 2));
		return dist < 0.1f;
	}

	bool checkCharCollisionDOWN() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ + charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 0.1f;
	}

	bool checkCharCollisionRIGHT() {
		float dist = sqrt(pow(charPosX + charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 0.1f;
	}

	bool checkCharCollisionLEFT() {
		float dist = sqrt(pow(charPosX - charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 0.1f;
	}

	void die() {
		isAlive = false;
		score += 10;
	}
};

class Bush {
public:
	Vector position;
	bool isVisible;
	float size;
	float renderScale;
	Bush() : position(0, 0, 0), isVisible(false), size(10.0f), renderScale(1.0f) {}
	Bush(float x, float y, float z, float _size, float _renderScale)
		: position(x, y, z), isVisible(true), size(_size), renderScale(_renderScale) {}
	void render() {
		if (isVisible) {
			glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glScalef(renderScale, renderScale, renderScale); // Scale for rendering
			model_bush.Draw();
			glPopMatrix();

		}
	}
	//collision for tree(obstacle) & character (undone)
	bool checkCollision(const Projectile& p) {
		float dist = sqrt(pow(p.position.x - position.x, 2) +
			pow(p.position.y - position.y, 2) +
			pow(p.position.z - position.z, 2));
		return dist < size + p.size;
	}

	bool checkCharCollisionUP() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ - charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	bool checkCharCollisionDOWN() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ + charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	bool checkCharCollisionRIGHT() {
		float dist = sqrt(pow(charPosX + charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	bool checkCharCollisionLEFT() {
		float dist = sqrt(pow(charPosX - charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

};

class Tree {
public:
	Vector position;
	bool isVisible;
	float size;
	float renderScale;
	Tree() : position(0, 0, 0), isVisible(false), size(10.0f), renderScale(1.0f) {}
	Tree(float x, float y, float z, float _size, float _renderScale)
		: position(x, y, z), isVisible(true), size(_size), renderScale(_renderScale) {}
	void render() {
		if (isVisible) {
			glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glScalef(renderScale, renderScale, renderScale); // Scale for rendering
			model_tree.Draw();
			glPopMatrix();

		}
	}
	//collision for tree(obstacle) & character (undone)
	bool checkCollision(const Projectile& p) {
		float dist = sqrt(pow(p.position.x - position.x, 2) +
			pow(p.position.y - position.y, 2) +
			pow(p.position.z - position.z, 2));
		return dist < size + p.size;
	}

	bool checkCharCollisionUP() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ - charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	bool checkCharCollisionDOWN() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ + charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	bool checkCharCollisionRIGHT() {
		float dist = sqrt(pow(charPosX + charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	bool checkCharCollisionLEFT() {
		float dist = sqrt(pow(charPosX - charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

};

class Aid {
public:
	Vector position;
	bool isVisible;
	float size;
	float renderScale;
	//size is TOO BIG TOO TOO TOO TOO BIG , (msh 3rfa 28iaro, w law 3mltlo scale el collision hatfr2a3)
	// (el collision el ana m3mlthash aywa) brdo fel zombies :)))
	Aid() : position(0, 0, 0), isVisible(false), size(10.0f), renderScale(1.0f) {}
	Aid(float x, float y, float z, float _size, float _renderScale)
		: position(x, y, z), isVisible(true), size(_size), renderScale(_renderScale) {}

	void render() {
		if (isVisible) {
			glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glScalef(renderScale, renderScale, renderScale); // Scale for rendering
			model_aid.Draw();
			glPopMatrix();
		}
	}
	//collision for collectibles (undone) + update it to vanish, only for show
	bool checkCollision(const Projectile& p) {
		float dist = sqrt(pow(p.position.x - position.x, 2) +
			pow(p.position.y - position.y, 2) +
			pow(p.position.z - position.z, 2));
		return dist < size + p.size;
	}

	bool checkCharCollisionUP() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ - charSpeed - position.z, 2));
		//std::cout << "char: " << charPosX << std::endl;
		//std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	bool checkCharCollisionDOWN() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ + charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	bool checkCharCollisionRIGHT() {
		float dist = sqrt(pow(charPosX + charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	bool checkCharCollisionLEFT() {
		float dist = sqrt(pow(charPosX - charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	void collect() {
		isVisible = false;

	}

};

class Barrel {
public:
	Vector position;
	bool isVisible;
	float size;
	float renderScale;
	Barrel() : position(0, 0, 0), isVisible(false), size(10.0f), renderScale(1.0f) {}
	Barrel(float x, float y, float z, float _size, float _renderScale)
		: position(x, y, z), isVisible(true), size(_size), renderScale(_renderScale) {}
	void render() {
		if (isVisible) {
			glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glScalef(renderScale, renderScale, renderScale); // Scale for rendering
			model_barrel.Draw();
			glPopMatrix();

		}
	}

	bool checkCharCollisionUP() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ - charSpeed - position.z, 2));
		//std::cout << "char: " << size << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		//std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size;
	}

	bool checkCharCollisionDOWN() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ + charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size;
	}

	bool checkCharCollisionRIGHT() {
		float dist = sqrt(pow(charPosX + charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size;
	}

	bool checkCharCollisionLEFT() {
		float dist = sqrt(pow(charPosX - charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size;
	}
};

class Coin {
public:
	Vector position;
	bool isVisible;
	float size;
	float renderScale;
	float rotationAngle;
	Coin() : position(0, 0, 0), isVisible(false), size(10.0f), renderScale(1.0f), rotationAngle(0.0f) {}
	Coin(float x, float y, float z, float _size, float _renderScale)
		: position(x, y, z), isVisible(true), size(_size), renderScale(_renderScale), rotationAngle(-90.0f) {}
	void render() {
		if (isVisible) {
			glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glRotatef(rotationAngle, 1.0f, 0.0f, 0.0f);
			glScalef(renderScale, renderScale, renderScale); // Scale for rendering
			model_coin.Draw();
			glPopMatrix();

		}
	}
	//collision for brdo a collectible :)))
	bool checkCollision(const Projectile& p) {
		float dist = sqrt(pow(p.position.x - position.x, 2) +
			pow(p.position.y - position.y, 2) +
			pow(p.position.z - position.z, 2));
		return dist < size + p.size;
	}

	bool checkCharCollisionUP() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ - charSpeed - position.z, 2));
		/*std::cout << "char: " << charPosX << std::endl;
		std::cout << "charz: " << charPosZ << std::endl;
		std::cout << "posx: " << position.x << std::endl;
		std::cout << "posz: " << position.z << std::endl;*/
		return dist < 3.0f;
	}

	bool checkCharCollisionDOWN() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ + charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	bool checkCharCollisionRIGHT() {
		float dist = sqrt(pow(charPosX + charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	bool checkCharCollisionLEFT() {
		float dist = sqrt(pow(charPosX - charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 3.0f;
	}

	void collect() {
		isVisible = false;
	}
};

class Syringe {
public:
	Vector position;
	bool isVisible;
	float size;
	float renderScale;
	Syringe() : position(0, 0, 0), isVisible(false), size(10.0f), renderScale(1.0f) {}
	Syringe(float x, float y, float z, float _size, float _renderScale) : position(x, y, z), isVisible(true), size(_size), renderScale(_renderScale) {}
	void render() {
		if (isVisible) {
			glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glScalef(renderScale, renderScale, renderScale); // Scale for rendering
			model_syringe.Draw();
			glPopMatrix();

		}
	}
	//collision for brdo a collectible :)))
	bool checkCollision(const Projectile& p) {
		float dist = sqrt(pow(p.position.x - position.x, 2) +
			pow(p.position.y - position.y, 2) +
			pow(p.position.z - position.z, 2));
		return dist < size + p.size;
	}
	bool checkCharCollisionUP() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ - charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 5.0f;
	}

	bool checkCharCollisionDOWN() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ + charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 5.0f;
	}

	bool checkCharCollisionRIGHT() {
		float dist = sqrt(pow(charPosX + charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 5.0f;
	}

	bool checkCharCollisionLEFT() {
		float dist = sqrt(pow(charPosX - charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < 5.0f;
	}

	void collect() {
		isVisible = false;
	}
};

class Truck {
public:
	Vector position;
	bool isVisible;
	float size;
	float renderScale;
	Truck() : position(0, 0, 0), isVisible(false), size(10.0f), renderScale(1.0f) {}
	Truck(float x, float y, float z, float _size, float _renderScale) : position(x, y, z), isVisible(true), size(_size), renderScale(_renderScale) {}
	void render() {
		if (isVisible) {
			glPushMatrix();
			glTranslatef(position.x, position.y, position.z);
			glScalef(renderScale, renderScale, renderScale); // Scale for rendering
			model_truck.Draw();
			glPopMatrix();

		}
	}
	//collision for brdo an obstacle mn el 2a5er m3mltsh ay collision leeh 3elaka bel charx w el chary:)))
	bool checkCollision(const Projectile& p) {
		float dist = sqrt(pow(p.position.x - position.x, 2) +
			pow(p.position.y - position.y, 2) +
			pow(p.position.z - position.z, 2));
		return dist < size + p.size;
	}
	bool checkCharCollisionUP() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ - charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size + 2.0f;
	}

	bool checkCharCollisionDOWN() {
		float dist = sqrt(pow(charPosX - position.x, 2) +
			pow(charPosZ + charSpeed - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size + 2.0f;
	}

	bool checkCharCollisionRIGHT() {
		float dist = sqrt(pow(charPosX + charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size + 2.0f;
	}

	bool checkCharCollisionLEFT() {
		float dist = sqrt(pow(charPosX - charSpeed - position.x, 2) +
			pow(charPosZ - position.z, 2));
		////std::cout << "char: " << charPosX << std::endl;
		//std::cout << "charz: " << charPosZ << std::endl;
		////std::cout << "posx: " << position.x << std::endl;
		//std::cout << "posz: " << position.z << std::endl;
		return dist < size + 2.0f;
	}
};





const int MAX_RHINOS = 5;
const int MAX_ZOMBIES = 5;
const int MAX_BUSHES = 5;
const int MAX_TREES = 5;
const int MAX_AIDS = 3;
const int MAX_BARRELS = 1;
const int MAX_COINS = 5;
const int MAX_SYRINGES = 5;
const int MAX_TRUCKS = 5;



Rhino rhinos[MAX_RHINOS];
Zombie zombies[MAX_ZOMBIES];
Bush bushes[MAX_BUSHES];
Tree trees[MAX_TREES];
Aid aids[MAX_AIDS];
Barrel barrels[MAX_BARRELS];
Coin coins[MAX_COINS];
Syringe syringes[MAX_SYRINGES];
Truck trucks[MAX_TRUCKS];

int numRhinos = 0;
int numZombies = 0;
int numBushes = 0;
int numTrees = 0;
int numAids = 0;
int numBarrels = 0;
int numCoins = 0;
int numSyringes = 0;
int numTrucks = 0;

//void initializeRhinos() {
//	for (int i = 0; i < MAX_RHINOS; i++) {
//		rhinos[i] = Rhino(rand() % 200, 0.0f, rand() % 200, 2.0f); // Random positions
//		numRhinos++;
//	}
//}

// Initialize rhinos and zombies at the beginning of the game
void initializeRhinos() {
	float sceneWidth = 150.0f;  // Width of the ground
	float sceneDepth = 150.0f;  // Depth of the ground

	for (int i = 0; i < MAX_RHINOS; i++) {
		float x = static_cast<float>(rand() % static_cast<int>(sceneWidth)) - sceneWidth / 2;
		float z = static_cast<float>(rand() % static_cast<int>(sceneDepth)) - sceneDepth / 2;
		float y = 0.0f; // Match the ground height
		rhinos[i] = Rhino(x, y, z, 2.0f); // Place rhino at (x, y, z)
		numRhinos++;
	}
}


void initializeZombies() {
	float sceneWidth = 100.0f;  // Width of the ground
	float sceneDepth = 100.0f;  // Depth of the ground
	float baseSize = 2.0f;
	float scaleFactor = 4.0f;

	for (int i = 0; i < MAX_ZOMBIES; i++) {
		float x = static_cast<float>(rand() % static_cast<int>(sceneWidth)) - sceneWidth / 2;
		float z = static_cast<float>(rand() % static_cast<int>(sceneDepth)) - sceneDepth / 2;
		float y = 0.0f; // Match the ground height

		zombies[i] = Zombie(x, y, z, baseSize * scaleFactor, scaleFactor); // Random positions
		numZombies++;




	}

}

void initializeBushes() {
	float sceneWidth = 150.0f;  // Width of the ground
	float sceneDepth = 150.0f;  // Depth of the ground
	float baseSize = 2.0f;
	float scaleFactor = 0.3f;
	for (int i = 0; i < MAX_BUSHES; i++) {
		float x = static_cast<float>(rand() % static_cast<int>(sceneWidth)) - sceneWidth / 2;
		float z = static_cast<float>(rand() % static_cast<int>(sceneDepth)) - sceneDepth / 2;
		float y = 0.0f; // Match the ground height
		bushes[i] = Bush(x, y, z, baseSize * scaleFactor, scaleFactor); // Random positions
		numBushes++;
	}
}

void initializeTrees() {
	float sceneWidth = 150.0f;  // Width of the ground
	float sceneDepth = 150.0f;  // Depth of the ground
	float baseSize = 2.0f;
	float scaleFactor = 1.0f;
	for (int i = 0; i < MAX_TREES; i++) {
		float x = static_cast<float>(rand() % static_cast<int>(sceneWidth)) - sceneWidth / 2;
		float z = static_cast<float>(rand() % static_cast<int>(sceneDepth)) - sceneDepth / 2;
		float y = 0.0f; // Match the ground height
		trees[i] = Tree(x, y, z, baseSize * scaleFactor, scaleFactor); // Random positions
		numTrees++;
	}
}

void initializeAids() {
	float sceneWidth = 150.0f;  // Width of the ground
	float sceneDepth = 150.0f;  // Depth of the ground
	float baseSize = 2.0f;
	float scaleFactor = 0.01f; // Adjust visually
	for (int i = 0; i < MAX_AIDS; i++) {
		float x = static_cast<float>(rand() % static_cast<int>(sceneWidth)) - sceneWidth / 2;
		float z = static_cast<float>(rand() % static_cast<int>(sceneDepth)) - sceneDepth / 2;
		float y = 0.0f; // Match the ground height
		std::cout << "xxxxxxxx: " << x << std::endl;

		aids[i] = Aid(x, y, z, baseSize * scaleFactor, scaleFactor);
		numAids++;
	}
}


void initializeBarrels() {
	float sceneWidth = 150.0f;  // Width of the ground
	float sceneDepth = 150.0f;  // Depth of the ground
	float baseSize = 2.0f;
	float scaleFactor = 2.0f; // Adjust visually
	for (int i = 0; i < MAX_BARRELS; i++) {
		float x = static_cast<float>(rand() % static_cast<int>(sceneWidth)) - sceneWidth / 2;
		float z = static_cast<float>(rand() % static_cast<int>(sceneDepth)) - sceneDepth / 2;
		float y = 0.0f; // Match the ground height
		barrels[i] = Barrel(x, y, z, baseSize * scaleFactor, scaleFactor); // Random positions
		numBarrels++;
	}
}

void initializeCoins() {
	float sceneWidth = 150.0f;  // Width of the ground
	float sceneDepth = 150.0f;  // Depth of the ground
	float baseSize = 2.0f;
	float scaleFactor = 0.3f;
	for (int i = 0; i < MAX_COINS; i++) {
		float x = static_cast<float>(rand() % static_cast<int>(sceneWidth)) - sceneWidth / 2;
		float z = static_cast<float>(rand() % static_cast<int>(sceneDepth)) - sceneDepth / 2;
		float y = 0.0f; // Match the ground height
		coins[i] = Coin(x, y, z, baseSize * scaleFactor, scaleFactor); // Random positions
		numCoins++;
	}
}

void initializeSyringes() {
	float sceneWidth = 150.0f;  // Width of the ground
	float sceneDepth = 150.0f;  // Depth of the ground
	float baseSize = 2.0f;
	float scaleFactor = 0.3f;
	for (int i = 0; i < MAX_COINS; i++) {
		float x = static_cast<float>(rand() % static_cast<int>(sceneWidth)) - sceneWidth / 2;
		float z = static_cast<float>(rand() % static_cast<int>(sceneDepth)) - sceneDepth / 2;
		float y = 0.0f; // Match the ground height
		syringes[i] = Syringe(x, y, z, baseSize * scaleFactor, scaleFactor); // Random positions
		numSyringes++;
	}
}

void initializeTrucks() {
	float sceneWidth = 70.0f;  // Width of the ground
	float sceneDepth = 70.0f;  // Depth of the ground
	float baseSize = 2.0f;
	float scaleFactor = 5.0f;
	for (int i = 0; i < MAX_TRUCKS; i++) {
		float x = static_cast<float>(rand() % static_cast<int>(sceneWidth)) - sceneWidth / 2;
		float z = static_cast<float>(rand() % static_cast<int>(sceneDepth)) - sceneDepth / 2;
		float y = 0.0f; // Match the ground height
		trucks[i] = Truck(x, y, z, baseSize * scaleFactor, scaleFactor); // Random positions
		numTrucks++;
	}
}
int currentLevel = 0;       // 0 = Start Screen, 1 = Hunting Rhinos, 2 = Hunting Zombies


#define MAX_BOUNDRIES 1000
Barrel boundaryBarrels[MAX_BOUNDRIES];
int numBarrelsBound = 0;
Tree boundaryTrees[MAX_BOUNDRIES];
int numTreesBound = 0;

void initializeBoundaries() {
	float size = 10.0f;    // Size of the barrels
	float renderScale = 1.0f; // Scale for rendering the barrel
	float spacing = 20.0f; // Spacing between barrels
	if (currentLevel == 1) {
		// Bottom edge (-200, 200)
		for (float x = -200; x <= 200; x += spacing) {
			if (numTreesBound < MAX_BOUNDRIES) {
				boundaryTrees[numTreesBound++] = Tree(x, 0, -200, size, renderScale);
			}
		}

		// Top edge (-200, 200)
		for (float x = -200; x <= 200; x += spacing) {
			if (numTreesBound < MAX_BOUNDRIES) {
				boundaryTrees[numTreesBound++] = Tree(x, 0, 200, size, renderScale);
			}
		}

		// Left edge (-200, 200)
		for (float z = -200; z <= 200; z += spacing) {
			if (numTreesBound < MAX_BOUNDRIES) {
				boundaryTrees[numTreesBound++] = Tree(-200, 0, z, size, renderScale);
			}
		}

		// Right edge (-200, 200)
		for (float z = -200; z <= 200; z += spacing) {
			if (numTreesBound < MAX_BARRELS) {
				boundaryTrees[numTreesBound++] = Tree(200, 0, z, size, renderScale);
			}
		}
	}
	else {
		if (currentLevel == 2) {
			// Bottom edge (-200, 200)
			for (float x = -200; x <= 200; x += spacing) {
				if (numBarrelsBound < MAX_BOUNDRIES) {
					boundaryBarrels[numBarrelsBound++] = Barrel(x, 0, -200, size, renderScale);
				}
			}

			// Top edge (-200, 200)
			for (float x = -200; x <= 200; x += spacing) {
				if (numBarrelsBound < MAX_BOUNDRIES) {
					boundaryBarrels[numBarrelsBound++] = Barrel(x, 0, 200, size, renderScale);
				}
			}

			// Left edge (-200, 200)
			for (float z = -200; z <= 200; z += spacing) {
				if (numBarrelsBound < MAX_BOUNDRIES) {
					boundaryBarrels[numBarrelsBound++] = Barrel(-200, 0, z, size, renderScale);
				}
			}

			// Right edge (-200, 200)
			for (float z = -200; z <= 200; z += spacing) {
				if (numBarrelsBound < MAX_BOUNDRIES) {
					boundaryBarrels[numBarrelsBound++] = Barrel(200, 0, z, size, renderScale);
				}
			}
		}
	}
}

void renderBoundaries() {
	if (currentLevel == 1) {
		for (int i = 0; i < numTreesBound; i++) {
			boundaryTrees[i].render();
		}
	}
	if (currentLevel == 2) {
		for (int i = 0; i < numBarrels; i++) {
			boundaryBarrels[i].render();
		}
	}
}



// Global variables to control the game state
// Start Screen Logic
bool isStartScreen = true; // Flag to control the start screen

// Modified drawScoreboard for drawing any dynamic text
void drawScoreboard(float x, float y, int z, const char* text) {
	// Switch to orthographic projection for the text
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 600);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Set text color
	glColor3f(1.0f, 1.0f, 1.0f); // White text

	// Draw the actual text in fixed screen space
	glRasterPos2f(x, y); // Adjust coordinates
	for (int i = 0; text[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}

	// Restore previous projection and modelview matrices
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void drawStartScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set background color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark background for the start screen

	// Draw title and instructions using the existing drawScoreboard logic
	drawScoreboard(300, 300, -1, "Hunting Game");
	drawScoreboard(250, 200, -1, "Press 1 for Hunting Rhinos");
	drawScoreboard(250, 150, -1, "Press 2 for Hunting Zombies");

	glutSwapBuffers();
}






void startHuntingRhinos() {
	currentLevel = 1;  // Set the current level to Hunting Rhinos

	// Initialize rhinos and projectiles
	initializeRhinos();
	initializeBushes();
	initializeAids();
	initializeTrees();
	initializeCoins();
	//initializeBoundaries();
	numProjectiles = 0; // Reset projectiles
}

void startHuntingZombies() {
	currentLevel = 2;  // Set the current level to Hunting Zombies

	// Initialize zombies and projectiles
	initializeZombies();
	initializeBarrels();
	initializeCoins();
	initializeSyringes();
	initializeTrucks();
	//initializeBoundaries();
	numProjectiles = 0; // Reset projectiles
}

int health = 25;
boolean collisionUP = false;
boolean collisionDOWN = false;
boolean collisionRIGHT = false;
boolean collisionLEFT = false;
boolean collisionUPOPP = false;
boolean collisionDOWNOPP = false;
boolean collisionRIGHTOPP = false;
boolean collisionLEFTOPP = false;
void updateZombies() {
	for (int i = 0; i < numProjectiles; ++i) {
		for (int j = 0; j < numZombies; j++) {
			if (zombies[j].isAlive && zombies[j].checkCollision(projectiles[i])) {
				zombies[j].die();
				projectiles[i].isActive = false;  // Deactivate projectile
				engine3->play2D("media/zombieDie.mp3", false);

			}
		}
	}

	collisionUPOPP = false;collisionDOWNOPP = false;collisionLEFTOPP = false;collisionRIGHTOPP = false;


	for (int i = 0; i < numZombies; i++) {
		zombies[i].render();
		float difference = std::abs(charPosX - zombies[i].position.x);
		float difference2 = std::abs(charPosZ - zombies[i].position.z);
		if (difference < 15.0f || difference2 < 15.0f) {
			if (charPosX > zombies[i].position.x) {
				zombies[i].position.x += 0.07f;
			}
			else {
				zombies[i].position.x -= 0.07f;
			}
			if (charPosZ > zombies[i].position.z) {
				zombies[i].position.z += 0.07f;
			}
			else {
				zombies[i].position.z -= 0.07f;
			}
		}
		if (zombies[i].isAlive && zombies[i].checkCharCollisionUP()) {
			health -= 0.1;
			collisionUPOPP = true;
		}

		if (zombies[i].isAlive && zombies[i].checkCharCollisionDOWN()) {
			health -= 0.1;
			collisionDOWNOPP = true;
		}

		if (zombies[i].isAlive && zombies[i].checkCharCollisionLEFT()) {
			health -= 0.1;
			collisionLEFTOPP = true;
		}

		if (zombies[i].isAlive && zombies[i].checkCharCollisionRIGHT()) {
			health -= 0.1;
			collisionRIGHTOPP = true;
		}
	}
}



void updateRhinos() {
	for (int i = 0; i < numProjectiles; ++i) {
		for (int j = 0; j < numRhinos; j++) {
			if (rhinos[j].isAlive && rhinos[j].checkCollision(projectiles[i])) {
				rhinos[j].die();  // Rhino dies
				projectiles[i].isActive = false;  // Deactivate projectile
				engine3->play2D("media/rhinoDie.mp3", false);
			}
		}
	}

	collisionUPOPP = false;collisionDOWNOPP = false;collisionLEFTOPP = false;collisionRIGHTOPP = false;


	for (int i = 0; i < numRhinos; i++) {
		rhinos[i].render();
		float difference = std::abs(charPosX - rhinos[i].position.x);
		float difference2 = std::abs(charPosZ - rhinos[i].position.z);
		if (difference < 15.0f || difference2 < 15.0f) {
			engine3->play2D("media/running.mp3", false);
			if (charPosX > rhinos[i].position.x) {
				rhinos[i].position.x += 0.1f;
			}
			else {
				rhinos[i].position.x -= 0.1f;
			}
			if (charPosZ > rhinos[i].position.z) {
				rhinos[i].position.z += 0.1f;
			}
			else {
				rhinos[i].position.z -= 0.1f;
			}
		}
		if (rhinos[i].isAlive && rhinos[i].checkCharCollisionUP()) {
			health -= 0.1;
			collisionUPOPP = true;
		}

		if (rhinos[i].isAlive && rhinos[i].checkCharCollisionDOWN()) {
			health -= 0.1;
			collisionDOWNOPP = true;
		}

		if (rhinos[i].isAlive && rhinos[i].checkCharCollisionLEFT()) {
			health -= 0.1;
			collisionLEFTOPP = true;
		}

		if (rhinos[i].isAlive && rhinos[i].checkCharCollisionRIGHT()) {
			health -= 0.1;
			collisionRIGHTOPP = true;
		}
	}
}


void updateAids() {




	for (int i = 0; i < numAids; i++) {
		aids[i].render();
		if (aids[i].isVisible && aids[i].checkCharCollisionUP()) {
			health += 5;
			aids[i].isVisible = false;
			aids[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}

		if (aids[i].isVisible && aids[i].checkCharCollisionDOWN()) {
			health += 5;
			aids[i].isVisible = false;
			aids[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}

		if (aids[i].isVisible && aids[i].checkCharCollisionLEFT()) {
			health += 5;
			aids[i].isVisible = false;
			aids[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}

		if (aids[i].isVisible && aids[i].checkCharCollisionRIGHT()) {
			health += 5;
			aids[i].isVisible = false;
			aids[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}
	}
}

void updateSyringes() {




	for (int i = 0; i < numSyringes; i++) {
		syringes[i].render();
		if (syringes[i].isVisible && syringes[i].checkCharCollisionUP()) {
			health += 5;
			syringes[i].isVisible = false;
			syringes[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}

		if (syringes[i].isVisible && syringes[i].checkCharCollisionDOWN()) {
			health += 5;
			syringes[i].isVisible = false;
			syringes[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}

		if (syringes[i].isVisible && syringes[i].checkCharCollisionLEFT()) {
			health += 5;
			syringes[i].isVisible = false;
			syringes[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}

		if (syringes[i].isVisible && syringes[i].checkCharCollisionRIGHT()) {
			health += 5;
			syringes[i].isVisible = false;
			syringes[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}
	}
}

void updateCoins() {




	for (int i = 0; i < numCoins; i++) {
		coins[i].render();
		if (coins[i].isVisible && coins[i].checkCharCollisionUP()) {
			score += 10;
			coins[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}

		if (coins[i].isVisible && coins[i].checkCharCollisionDOWN()) {
			score += 10;
			coins[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}

		if (coins[i].isVisible && coins[i].checkCharCollisionLEFT()) {
			score += 10;
			coins[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}

		if (aids[i].isVisible && aids[i].checkCharCollisionRIGHT()) {
			score += 10;
			coins[i].collect();
			engine3->play2D("media/collectible.mp3", false);

		}
	}
}

void updateTrees() {
	collisionUP = false;collisionDOWN = false;collisionLEFT = false;collisionRIGHT = false;


	for (int i = 0; i < numTrees; i++) {
		trees[i].render();
		if (trees[i].isVisible && trees[i].checkCharCollisionUP()) {
			collisionUP = true;
		}

		if (trees[i].isVisible && trees[i].checkCharCollisionDOWN()) {

			collisionDOWN = true;
		}

		if (trees[i].isVisible && trees[i].checkCharCollisionLEFT()) {
			collisionLEFT = true;
		}

		if (trees[i].isVisible && trees[i].checkCharCollisionRIGHT()) {
			collisionRIGHT = true;
		}
	}
}



bool collisionUPBar = false; bool collisionDOWNBar = false; bool collisionLEFTBar = false; bool collisionRIGHTBar = false;

void updateBushes() {
	collisionUPBar = false;collisionDOWNBar = false;collisionLEFTBar = false;collisionRIGHTBar = false;


	for (int i = 0; i < numBushes; i++) {
		bushes[i].render();
		if (bushes[i].checkCharCollisionUP()) {
			collisionUPBar = true;
		}

		if (bushes[i].checkCharCollisionDOWN()) {

			collisionDOWNBar = true;
		}

		if (bushes[i].checkCharCollisionLEFT()) {
			collisionLEFTBar = true;
		}

		if (bushes[i].checkCharCollisionRIGHT()) {
			collisionRIGHTBar = true;
		}
	}
}

void updateBarells() {
	collisionUPBar = false;collisionDOWNBar = false;collisionLEFTBar = false;collisionRIGHTBar = false;


	for (int i = 0; i < numBarrels; i++) {
		barrels[i].render();
		if (barrels[i].checkCharCollisionUP()) {
			collisionUPBar = true;
		}

		if ( barrels[i].checkCharCollisionDOWN()) {

			collisionDOWNBar = true;
		}

		if ( barrels[i].checkCharCollisionLEFT()) {
			collisionLEFTBar = true;
		}

		if ( barrels[i].checkCharCollisionRIGHT()) {
			collisionRIGHTBar = true;
		}
	}
}

void updateTrucks() {
	collisionUP = false;collisionDOWN = false;collisionLEFT = false;collisionRIGHT = false;


	for (int i = 0; i < numTrucks; i++) {
		trucks[i].render();
		if (trucks[i].isVisible && trucks[i].checkCharCollisionUP()) {
			collisionUP = true;
		}

		if (trucks[i].isVisible && trucks[i].checkCharCollisionDOWN()) {

			collisionDOWN = true;
		}

		if (trucks[i].isVisible && trucks[i].checkCharCollisionLEFT()) {
			collisionLEFT = true;
		}

		if (trucks[i].isVisible && trucks[i].checkCharCollisionRIGHT()) {
			collisionRIGHT = true;
		}
	}
}



//update collectables for world 1 (aids) (make it disappear)+sheel mn display
//void updateAids() {
//
//}

//update coin collectables for both worlds+sheel mn display
//void updatecoins(){}

//update syrnges when collide with character+sheel mn el display when u update
//void updateSyringes(){}




// Function to shoot projectiles
void shootProjectile(int weaponType) {
	float velocityX = sin(playerRotationAngle * M_PI / 180.0) * 10.0f;
	float velocityZ = -cos(playerRotationAngle * M_PI / 180.0) * 10.0f;
	if (playerRotationAngle == 270.0f || playerRotationAngle == 90.0f) {
		velocityX *= -1;
		velocityZ *= -1;
	}
	if (numProjectiles < MAX_PROJECTILES) {
		projectiles[numProjectiles++] = Projectile(charPosX, charPosY + 1.5f, charPosZ, Vector(velocityX, 0.0f, velocityZ), weaponType);
	}
}


void drawHeart(float centerX, float centerY, float size) {
	glBegin(GL_POLYGON);  // Begin drawing the heart as a filled polygon

	// Loop through values of t to generate vertices
	for (float t = 0.0f; t <= 2 * 3.14; t += 0.01f) {
		float x = 16 * pow(sin(t), 3);  // Parametric equation for x
		float y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);  // Parametric equation for y

		// Scale and translate the heart to desired position
		glVertex2f(centerX + x * size, centerY + y * size);
	}

	glEnd();  // End drawing the heart
}

void RenderLives()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // Save the current projection matrix
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, 0, HEIGHT);  // 2D orthographic projection

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // Save the current modelview matrix
	glLoadIdentity();

	glDisable(GL_LIGHTING);  // Disable lighting for 2D rendering
	glColor3f(1.0, 0.0, 0.0);  // Set the color to red for the hearts/lives

	// Render the hearts/lives at the top of the screen
	for (int i = 0; i < health / 5; i++) {
		drawHeart(500 + i * 50, HEIGHT - 50, 1.0);  // Draw hearts with increased spacing
	}

	glEnable(GL_LIGHTING);  // Re-enable lighting after rendering the hearts/lives

	glPopMatrix(); // Restore the previous modelview matrix

	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); // Restore the previous projection matrix

	glMatrixMode(GL_MODELVIEW);  // Ensure the modelview matrix mode is active
}

void drawGameOver() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glColor3f(1.0f, 0.0f, 0.0f);  // Set color to red
	glRasterPos2f(300, 300);

	if (health <= 0) {
		drawScoreboard(300, 300, -1, "Game Over!");
		engine2->play2D("media/lose.mp3", false);
	}
	else {
		drawScoreboard(300, 300, -1, "Game End!");
		engine2->play2D("media/win.mp3", false);
	}

	glColor3f(1.0f, 1.0f, 1.0f);  // White color for score
	char scoreText[50];
	sprintf(scoreText, "Final Score: %d", score);
	drawScoreboard(300, 400, -1, scoreText);

	glColor3f(1.0f, 1.0f, 1.0f);  // Instructions
	drawScoreboard(300, 500, -1, "Press R to restart!");
	glutSwapBuffers();

}


//=======================================================================
// Display Function
//=======================================================================

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	//GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
	GLfloat lightIntensity1[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition1[] = { charPosX, 10.0f, charPosZ, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightIntensity1);
	setCamera();
	if (isStartScreen) {
		drawStartScreen();
		return;
	}
	if (health <= 4.9 || timerSeconds <= 0) {
		isOver = true;
		drawGameOver();
		return;
	}
	// Draw Ground
	RenderGround();
	renderBoundaries();

	// Draw Animated Character
	glPushMatrix();
	glTranslatef(charPosX, charPosY, charPosZ);
	glRotatef(playerRotationAngle, 0.0f, 1.0f, 0.0f); // Apply player rotation
	glScalef(0.07, 0.07, 0.07);

	// Render Body (Main Character)
	model_char.Draw();
	

	glPopMatrix();

	



	// Draw the current weapon
	glPushMatrix();
	glTranslatef(charPosX, charPosY, charPosZ);
	glRotatef(playerRotationAngle, 0.0f, 1.0f, 0.0f); // Apply player rotation
	switch (currentWeapon)
	{
	case 0: // Gun
		glTranslatef(2.1f, 4.2f, -1.5f); // Adjust the position relative to the character
		glScalef(0.02f, 0.02f, 0.02f); // Scale down the gun
		glRotatef(180.0f, 0, 1, 0); // Rotate the gun if necessary
		model_gun1.Draw();
		break;
	case 1: // Bow
		glTranslatef(2.1f, 4.2f, -1.5f); // Adjust the position relative to the character
		glScalef(0.01f, 0.01f, 0.01f); // Scale down the bow
		glRotatef(270.0f, 0, 1, 0); // Rotate the bow if necessary
		model_bow.Draw();
		break;
	case 2: // Knife
		glTranslatef(3.1f, 0.0f, -1.2f); // Adjust the position relative to the character
		glScalef(0.06f, 0.06f, 0.06f); // Scale down the knife
		//glRotatef(90.0f,0, 1, 0); // Rotate the knife if necessary
		model_knife.Draw();
		break;
	}
	glPopMatrix();

	if (currentLevel == 1) {
		updateBushes();
		updateRhinos();
		updateAids();

		updateTrees();
		updateCoins();
		glPushMatrix();
		glTranslatef(5, 10, 0);
		glScalef(0.2, 0.2, 0.2);
		model_sun.Draw();
		glPopMatrix();
		UpdateSun();

	}
	else if (currentLevel == 2) {
		updateZombies();

		updateCoins();
		updateBarells();
		updateSyringes();
		updateTrucks();
		UpdateSpotlight();

	}

	// Skybox (unchanged)
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 200, 200, 200);
	gluDeleteQuadric(qobj);


	glPopMatrix();

	// Render projectiles
	for (int i = 0; i < numProjectiles; ++i) {
		projectiles[i].render();
	}



	RenderTimer();
	RenderScore();
	RenderLives();

	glutSwapBuffers();
}



//=======================================================================
// Keyboard Function
//=======================================================================

void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
		// Start Screen Case
	case '1':  // Start level 1 (Hunting Rhinos)
		if (isStartScreen) {
			isStartScreen = false;
			startHuntingRhinos();  // Initialize level 1
		}
		break;
	case '2':  // Start level 2 (Hunting Zombies)
		if (isStartScreen) {
			isStartScreen = false;
			startHuntingZombies(); // Initialize level 2
		}
		break;

		// In-game Controls
	case 't':
		if (currentWeapon == 1) {
			engine2->play2D("media/bow.mp3", false);
		}
		if (currentWeapon == 0) {
			engine2->play2D("media/rifle.mp3", false);
		}
		if (currentWeapon == 2) {
			engine2->play2D("media/knife.mp3", false);
		}
		shootProjectile(currentWeapon);
		break;
	case 'n':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'm':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'r':
		if (isOver) {
			health = 25;
			timerSeconds = 20;
			currentLevel = 0;
			isStartScreen = true;
		}
		break;
	case 27: // Esc button
		exit(0);
		break;
	case 'w':
		if (!collisionUP && !collisionUPBar && !collisionUPOPP) {
			if (charPosZ - charSpeed > -200) {
				charPosZ -= charSpeed;
				isMoving = true; // Character is moving
			}
		}
		break;
	case 's':
		if (!collisionDOWN && !collisionDOWNBar && !collisionDOWNOPP) {
			if (charPosZ + charSpeed < 200) {
				charPosZ += charSpeed;
				isMoving = true; // Character is moving
			}
		}
		break;
	case 'a':
		if (!collisionLEFT && !collisionLEFTBar && !collisionLEFTOPP) {
			if (charPosX - charSpeed > -200) {
				charPosX -= charSpeed;
				isMoving = true; // Character is moving
			}
		}
		break;
	case 'd':
		if (!collisionRIGHT && !collisionRIGHTBar && !collisionRIGHTOPP) {
			if (charPosX + charSpeed < 200) {
				charPosX += charSpeed;
				isMoving = true; // Character is moving
			}
		}
		break;
	case 32: // Space bar
		if (!isJumping)
		{
			isJumping = true;
			charPosY += jumpHeight;
		}
		break;
	case 'c': // Toggle camera mode
		cameraMode = 1 - cameraMode; // Toggle between 0 (first-person) and 1 (third-person)
		break;
	case 'g': // Toggle between gun and bow
		currentWeapon = (currentWeapon + 1) % 3; // Cycle through 0, 1, 2
		break;
	case 'q': // Rotate player left
		playerRotationAngle -= 90.0f;
		if (playerRotationAngle < 0.0f)
		{
			playerRotationAngle += 360.0f;
		}
		break;
	default:
		isMoving = false; // Character stops moving for other keys
		break;
	}

	glutPostRedisplay();
}

void Update()
{
	if (isJumping)
	{
		// Simulate gravity
		charPosY -= 0.071f;
		if (charPosY <= 0.0f)
		{
			charPosY = 0.0f;
			isJumping = false;
			charPosZ -= 3.0f; // Adjust this value as needed

		}
	}
	if (cameraMode == 1) { // Third-person
		Eye.x = charPosX - 20 * cos(cameraAngleY * M_PI / 180.0) * sin(cameraAngleX * M_PI / 180.0);
		Eye.y = charPosY + 10.0f; // Height above player
		Eye.z = charPosZ - 20 * cos(cameraAngleY * M_PI / 180.0) * cos(cameraAngleX * M_PI / 180.0);

		At.x = charPosX;
		At.y = charPosY;
		At.z = charPosZ;
	}

	// Update projectiles
	for (int i = 0; i < numProjectiles; ++i) {
		projectiles[i].update(0.016f);  // Assuming 60 FPS, deltaTime ~ 1/60
	}



	glutPostRedisplay();
}

void myKeyboardUp(unsigned char button, int x, int y)
{
	// Stop movement when keys are released
	if (button == 'f' || button == 's' || button == 'a' || button == 'd')
	{
		isMoving = false;
	}
}


// Add these variables at the top of the file
int prevMouseX = 0;
int prevMouseY = 0;
bool isRotating = false;

// Update the myMotion function
void myMotion(int x, int y)
{
	y = HEIGHT - y; // Adjust for OpenGL's coordinate system

	if (isRotating)
	{
		int deltaX = x - prevMouseX;
		int deltaY = y - prevMouseY;

		// Adjust the camera angles
		cameraAngleX += deltaX * 0.1f;
		cameraAngleY += deltaY * 0.1f;

		// Limit the vertical angle to avoid flipping
		if (cameraAngleY > 89.0f) cameraAngleY = 89.0f;
		if (cameraAngleY < -89.0f) cameraAngleY = -89.0f;

		// Calculate new camera position based on the current mode
		if (cameraMode == 0) // First-person
		{
			GLdouble radX = cameraAngleX * M_PI / 180.0;
			GLdouble radY = cameraAngleY * M_PI / 180.0;

			At.x = Eye.x + cos(radY) * sin(radX);
			At.y = Eye.y + sin(radY);
			At.z = Eye.z + cos(radY) * cos(radX);
		}
		else if (cameraMode == 1) // Third-person
		{
			GLdouble radX = cameraAngleX * M_PI / 180.0;
			GLdouble radY = cameraAngleY * M_PI / 180.0;

			// Camera follows the player dynamically
			Eye.x = charPosX - 20 * cos(radY) * sin(radX);
			Eye.y = charPosY + 10; // Maintain a height above the player
			Eye.z = charPosZ - 20 * cos(radY) * cos(radX);

			At.x = charPosX;
			At.y = charPosY;
			At.z = charPosZ;
		}

		// Update previous mouse positions
		prevMouseX = x;
		prevMouseY = y;

		// Update the view
		glLoadIdentity();
		gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);

		// Optional: Adjust light position for dynamic effects
		GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		// Re-draw the scene
		glutPostRedisplay();
	}
}


// Update the myMouse function
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			isRotating = true;
			prevMouseX = x;
			prevMouseY = y;
		}
		else if (state == GLUT_UP)
		{
			isRotating = false;
		}
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);

}
//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_gun1.Load("Models/weapon/gun.3DS");
	model_bow.Load("Models/bow/bow.3DS");
	model_char.Load("Models/char/Man.3DS");
	model_knife.Load("Models/knife/Knife.3ds");
	model_aid.Load("Models/aid/untitled.3DS");
	model_barrel.Load("Models/barrel/Barrel.3DS");
	model_bush.Load("Models/bush1/Bush3.3DS");
	model_coin.Load("Models/coin/Coin.3DS");
	model_house.Load("Models/house/house.3DS");
	model_syringe.Load("Models/syringe/syringe.3DS");
	model_tree.Load("Models/tree/Tree1.3DS");
	model_truck.Load("Models/truck/truck.3DS");
	model_wall.Load("Models/wall/wall.3DS");
	model_zombie.Load("Models/zombie/zombie.3DS");
	model_rhino.Load("Models/rhino/rhino.3DS");
	model_sun.Load("Models/sun/sun.3DS");







	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}



void myIdle()
{
	static float lastTime = 0.0f;
	float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	updateAnimation(deltaTime);

	// Reset movement state after handling
	isMoving = false;

	glutPostRedisplay();
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	srand(static_cast<unsigned int>(time(0)));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 150);
	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myKeyboardUp); // Register the key release function
	glutMotionFunc(myMotion);
	glutMouseFunc(myMouse);
	glutReshapeFunc(myReshape);
	glutIdleFunc(Update); // Add idle function for animation


	myInit();
	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	/*glEnable(GL_LIGHT1);*/
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	glutTimerFunc(1000, TimerCallback, 0);
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	if (!engine) {
		return;
	}
	engine->setSoundVolume(0.07f);
	irrklang::ISound* sound = engine->play2D("media/main.mp3", true);

	glutMainLoop();
}