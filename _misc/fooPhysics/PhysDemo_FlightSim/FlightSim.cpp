/***************************************************************************************
*  Program Name:    FlightSim.cpp                                                      *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 14, 2013                                                  *
*  Description:     This file contains the definition file for the default application *
*                   object needed to run PhysDemo_FlightSim                            *
*                   - Showcases a different aspect of the Physics Engine: play Demo    *
*                             - Seperate Solution -                                    *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************//*
 * The flight Simulation demo.
 */

#include "../gl/glut.h"
#include "../PhysicsEngine/physicsengine/PhysicsEngine.h"
#include "../Common Files/timing.h"
#include "../Common Files/PhysApp.h"

#include <stdio.h>
#include <cassert>

/**
 * The main demo class definition.
 */
class FlightSimDemo : public Application
{
    PhysicsEngine::AeroControl left_wing;
    PhysicsEngine::AeroControl right_wing;
    PhysicsEngine::AeroControl rudder;
    PhysicsEngine::Aero tail;
    PhysicsEngine::PhysBody aircraft;
    PhysicsEngine::ForceRegistry registry;

    PhysicsEngine::Vector3D windspeed;

    float left_wing_control;
    float right_wing_control;
    float rudder_control;

    void resetPlane();

public:
    /** Creates a new demo object. */
    FlightSimDemo();
    virtual ~FlightSimDemo();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Display the particles. */
    virtual void display();

    /** Update the particle positions. */
    virtual void update();

    /** Handle a key press. */
    virtual void key(unsigned char key);
};

// Method definitions
FlightSimDemo::FlightSimDemo()
:
Application(),

right_wing(PhysicsEngine::Matrix3(0,0,0, -1,-0.5f,0, 0,0,0),
           PhysicsEngine::Matrix3(0,0,0, -0.995f,-0.5f,0, 0,0,0),
           PhysicsEngine::Matrix3(0,0,0, -1.005f,-0.5f,0, 0,0,0),
           PhysicsEngine::Vector3D(-1.0f, 0.0f, 2.0f), &windspeed),

left_wing(PhysicsEngine::Matrix3(0,0,0, -1,-0.5f,0, 0,0,0),
          PhysicsEngine::Matrix3(0,0,0, -0.995f,-0.5f,0, 0,0,0),
          PhysicsEngine::Matrix3(0,0,0, -1.005f,-0.5f,0, 0,0,0),
          PhysicsEngine::Vector3D(-1.0f, 0.0f, -2.0f), &windspeed),

rudder(PhysicsEngine::Matrix3(0,0,0, 0,0,0, 0,0,0),
       PhysicsEngine::Matrix3(0,0,0, 0,0,0, 0.01f,0,0),
       PhysicsEngine::Matrix3(0,0,0, 0,0,0, -0.01f,0,0),
       PhysicsEngine::Vector3D(2.0f, 0.5f, 0), &windspeed),

tail(PhysicsEngine::Matrix3(0,0,0, -1,-0.5f,0, 0,0,-0.1f),
     PhysicsEngine::Vector3D(2.0f, 0, 0), &windspeed),

left_wing_control(0), right_wing_control(0), rudder_control(0),

windspeed(0,0,0)
{
    // Set up the aircraft rigid body.
    resetPlane();

    aircraft.setBodyMass(2.5f);
    PhysicsEngine::Matrix3 it;
    it.SetBlockInertiaTensor(PhysicsEngine::Vector3D(2,1,1), 1);
    aircraft.setInertiaTensor(it);

    aircraft.setObjectDamping(0.8f, 0.8f);

    aircraft.setBodyAcceleration(PhysicsEngine::Vector3D::GRAVITY);
    aircraft.calculateObjectData();

    aircraft.setObjectStatis();
    aircraft.setCanGoIntoStatis(false);

    registry.addToRegistry(&aircraft, &left_wing);
    registry.addToRegistry(&aircraft, &right_wing);
    registry.addToRegistry(&aircraft, &rudder);
    registry.addToRegistry(&aircraft, &tail);
}

FlightSimDemo::~FlightSimDemo()
{
}

void FlightSimDemo::resetPlane()
{
    aircraft.setBodyPosition(0, 0, 0);
    aircraft.setBodyOrientation(1,0,0,0);

    aircraft.setObjectVelocity(0,0,0);
    aircraft.setObjectRotation(0,0,0);
}

static void drawAircraft()
{
    // Fuselage
    glPushMatrix();
    glTranslatef(-0.5f, 0, 0);
    glScalef(2.0f, 0.8f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Rear Fuselage
    glPushMatrix();
    glTranslatef(1.0f, 0.15f, 0);
    glScalef(2.75f, 0.5f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Wings
    glPushMatrix();
    glTranslatef(0, 0.3f, 0);
    glScalef(0.8f, 0.1f, 6.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Rudder
    glPushMatrix();
    glTranslatef(2.0f, 0.775f, 0);
    glScalef(0.75f, 1.15f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Tail-plane
    glPushMatrix();
    glTranslatef(1.9f, 0, 0);
    glScalef(0.85f, 0.1f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void FlightSimDemo::display()
{
    // Clear the view port and set the camera direction
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    PhysicsEngine::Vector3D pos = aircraft.getBodyPosition();
    PhysicsEngine::Vector3D offset(4.0f+aircraft.getObjectVelocity().magnitude(), 0, 0);
    offset = aircraft.getTransform().transformDirection(offset);
    gluLookAt(pos.x+offset.x, pos.y+5.0f, pos.z+offset.z,
              pos.x, pos.y, pos.z,
              0.0, 1.0, 0.0);

    glColor3f(0.6f,0.6f,0.6f);
    int bx = int(pos.x);
    int bz = int(pos.z);
    glBegin(GL_QUADS);
    for (int x = -20; x <= 20; x++) for (int z = -20; z <= 20; z++)
    {
        glVertex3f(bx+x-0.1f, 0, bz+z-0.1f);
        glVertex3f(bx+x-0.1f, 0, bz+z+0.1f);
        glVertex3f(bx+x+0.1f, 0, bz+z+0.1f);
        glVertex3f(bx+x+0.1f, 0, bz+z-0.1f);
    }
    glEnd();

    // Set the transform matrix for the aircraft
    PhysicsEngine::Matrix4 transform = aircraft.getTransform();
    GLfloat gl_transform[16];
    transform.fillGLArray(gl_transform);
    glPushMatrix();
    glMultMatrixf(gl_transform);

    // Draw the aircraft
    glColor3f(0,0,0);
    drawAircraft();
    glPopMatrix();

    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(0, -1.0f - pos.y, 0);
    glScalef(1.0f, 0.001f, 1.0f);
    glMultMatrixf(gl_transform);
    drawAircraft();
    glPopMatrix();

    char buffer[256];
    sprintf_s(
        buffer,
        "Altitude: %.1f | Speed %.1f",
        aircraft.getBodyPosition().y,
        aircraft.getObjectVelocity().magnitude()
        );
    glColor3f(0,0,0);
    renderText(10.0f, 24.0f, buffer);

    sprintf_s(
        buffer,
        "Left Wing: %.1f | Right Wing: %.1f | Rudder %.1f",
        left_wing_control, right_wing_control, rudder_control
        );
    renderText(10.0f, 10.0f, buffer);
}

void FlightSimDemo::update()
{
    // Find the duration of the last frame in seconds
    double dElapsedFrameTime = (double)TimingData::get().lastFrameDuration * 0.001;
    if (dElapsedFrameTime <= 0.0f) return;

    // Start with no forces or acceleration.
    aircraft.emptyAccumulators();

    // Add the propeller force
    PhysicsEngine::Vector3D propulsion(-10.0, 0, 0);
    propulsion = aircraft.getTransform().transformDirection(propulsion);
    aircraft.addForceToObject(propulsion);

    // Add the forces acting on the aircraft.
    registry.updateForcesOnBodies(dElapsedFrameTime);

    // Update the aircraft's physics.
    aircraft.physicsIntegration(dElapsedFrameTime);

    // Do a very basic collision detection and response with the ground.
    PhysicsEngine::Vector3D pos = aircraft.getBodyPosition();
    if (pos.y < 0.0f)
    {
        pos.y = 0.0f;
        aircraft.setBodyPosition(pos);

        if (aircraft.getObjectVelocity().y < -10.0f)
        {
            resetPlane();
        }
    }

    Application::update();
}

const char* FlightSimDemo::getTitle()
{
    return "PhysicsEngine><RABBID>< Flight Simulation Demo";
}

void FlightSimDemo::key(unsigned char key)
{
    switch(key)
    {
    case 'q': case 'Q':
        rudder_control += 0.1f;
        break;

    case 'e': case 'E':
        rudder_control -= 0.1f;
        break;

    case 'w': case 'W':
        left_wing_control -= 0.1f;
        right_wing_control -= 0.1f;
        break;

    case 's': case 'S':
        left_wing_control += 0.1f;
        right_wing_control += 0.1f;
        break;

    case 'd': case 'D':
        left_wing_control -= 0.1f;
        right_wing_control += 0.1f;
        break;

    case 'a': case 'A':
        left_wing_control += 0.1f;
        right_wing_control -= 0.1f;
        break;

    case 'x': case 'X':
        left_wing_control = 0.0f;
        right_wing_control = 0.0f;
        rudder_control = 0.0f;
        break;

    case 'r': case 'R':
        resetPlane();
        break;

    default:
        Application::key(key);
    }

    // Make sure the controls are in range
    if (left_wing_control < -1.0f) left_wing_control = -1.0f;
    else if (left_wing_control > 1.0f) left_wing_control = 1.0f;
    if (right_wing_control < -1.0f) right_wing_control = -1.0f;
    else if (right_wing_control > 1.0f) right_wing_control = 1.0f;
    if (rudder_control < -1.0f) rudder_control = -1.0f;
    else if (rudder_control > 1.0f) rudder_control = 1.0f;

    // Update the control surfaces
    left_wing.setControl(left_wing_control);
    right_wing.setControl(right_wing_control);
    rudder.setControl(rudder_control);
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
    return new FlightSimDemo();
}