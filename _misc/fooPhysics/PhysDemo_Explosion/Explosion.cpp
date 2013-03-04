/***************************************************************************************
*  Program Name:    Explosion.cpp                                                      *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 14, 2013                                                  *
*  Description:     This file contains the definition file for the default application *
*                   object needed to run PhysDemo_Explosion                            *
*                   - Showcases a different aspect of the Physics Engine: play Demo    *
*                             - Seperate Solution -                                    *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * The explosion demo.
*/

#include "../gl/glut.h"
#include "../PhysicsEngine/physicsengine/PhysicsEngine.h"
#include "../Common Files/timing.h"
#include "../Common Files/PhysApp.h"

#include <stdio.h>

#define OBJECTS 5

// Holds a transform matrix for rendering objects
// reflected in the floor.
GLfloat floorMirror[16] =
{
    1, 0, 0, 0,
    0, -1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

class Ball : public PhysicsEngine::CollisionSphere
{
public:
    Ball()
    {
        pBody = new PhysicsEngine::PhysBody;
    }

    ~Ball()
    {
        delete pBody;
    }

    /** Draws the box, excluding its shadow. */
    void render()
    {
        // Get the OpenGL transformation
        GLdouble mat[16];
        pBody->getGLTransform(mat);

        if (pBody->getObjectStatis()) glColor3f(1.0f,0.7f,0.7f);
        else glColor3f(0.7f,0.7f,1.0f);

        glPushMatrix();
        glMultMatrixd(mat);
        glutSolidSphere(m_dSphereRadius, 20, 20);
        glPopMatrix();
    }

    /** Draws the ground plane shadow for the box. */
    void renderShadow()
    {
        // Get the OpenGL transformation
        GLdouble mat[16];
        pBody->getGLTransform(mat);

        glPushMatrix();
        glScalef(1.0f, 0, 1.0f);
        glMultMatrixd(mat);
        glutSolidSphere(m_dSphereRadius, 20, 20);
        glPopMatrix();
    }

    /** Sets the box to a specific location. */
    void setState(PhysicsEngine::Vector3D position,
                  PhysicsEngine::Quaternion orientation,
                  double m_dSphereRadius,
                  PhysicsEngine::Vector3D velocity)
    {
        pBody->setBodyPosition(position);
        pBody->setBodyOrientationQ(orientation);
        pBody->setObjectVelocity(velocity);
        pBody->setObjectRotation(PhysicsEngine::Vector3D(0,0,0));
        Ball::m_dSphereRadius = m_dSphereRadius;

        double mass = 4.0f*0.3333f*3.1415f * m_dSphereRadius*m_dSphereRadius*m_dSphereRadius;
        pBody->setBodyMass(mass);

        PhysicsEngine::Matrix3 tensor;
        double coeff = 0.4f*mass*m_dSphereRadius*m_dSphereRadius;
        tensor.SetInertiaTensorCoeffs(coeff,coeff,coeff);
        pBody->setInertiaTensor(tensor);

        pBody->setObjectLinearDamping(0.95f);
        pBody->setObjectAngularDamping(0.8f);
        pBody->emptyAccumulators();
        pBody->setBodyAcceleration(0,-10.0f,0);

        //body->setCanSleep(false);
        pBody->setObjectStatis();

        pBody->calculateObjectData();
    }

    /** Positions the box at a random location. */
    void random(PhysicsEngine::Random *random)
    {
        const static PhysicsEngine::Vector3D minPos(-5, 5, -5);
        const static PhysicsEngine::Vector3D maxPos(5, 10, 5);
        PhysicsEngine::Random r;
        setState(
            random->randomVector(minPos, maxPos),
            random->randomQuaternion(),
            random->randomDouble(0.5f, 1.5f),
            PhysicsEngine::Vector3D()
            );
    }
};

class Box : public PhysicsEngine::CollisionBox
{
public:
    bool isOverlapping;

    Box()
    {
        pBody = new PhysicsEngine::PhysBody;
    }

    ~Box()
    {
        delete pBody;
    }

    /** Draws the box, excluding its shadow. */
    void render()
    {
        // Get the OpenGL transformation
        GLdouble mat[16];
        pBody->getGLTransform(mat);

        if (isOverlapping) glColor3f(0.7f,1.0f,0.7f);
        else if (pBody->getObjectStatis()) glColor3f(1.0f,0.7f,0.7f);
        else glColor3f(0.7f,0.7f,1.0f);

        glPushMatrix();
        glMultMatrixd(mat);
        glScaled(m_v3HalfSizeBox.x*2, m_v3HalfSizeBox.y*2, m_v3HalfSizeBox.z*2);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    /** Draws the ground plane shadow for the box. */
    void renderShadow()
    {
        // Get the OpenGL transformation
        GLdouble mat[16];
        pBody->getGLTransform(mat);

        glPushMatrix();
        glScaled(1.0, 0, 1.0);
        glMultMatrixd(mat);
        glScaled(m_v3HalfSizeBox.x*2, m_v3HalfSizeBox.y*2, m_v3HalfSizeBox.z*2);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    /** Sets the box to a specific location. */
    void setState(const PhysicsEngine::Vector3D &position,
                  const PhysicsEngine::Quaternion &orientation,
                  const PhysicsEngine::Vector3D &extents,
                  const PhysicsEngine::Vector3D &velocity)
    {
        pBody->setBodyPosition(position);
        pBody->setBodyOrientationQ(orientation);
        pBody->setObjectVelocity(velocity);
        pBody->setObjectRotation(PhysicsEngine::Vector3D(0,0,0));
        m_v3HalfSizeBox = extents;

        double mass = m_v3HalfSizeBox.x * m_v3HalfSizeBox.y * m_v3HalfSizeBox.z * 8.0f;
        pBody->setBodyMass(mass);

        PhysicsEngine::Matrix3 tensor;
        tensor.SetBlockInertiaTensor(m_v3HalfSizeBox, mass);
        pBody->setInertiaTensor(tensor);

        pBody->setObjectLinearDamping(0.95f);
        pBody->setObjectAngularDamping(0.8f);
        pBody->emptyAccumulators();
        pBody->setBodyAcceleration(0,-10.0f,0);

        pBody->setObjectStatis();

        pBody->calculateObjectData();
    }

    /** Positions the box at a random location. */
    void random(PhysicsEngine::Random *random)
    {
        const static PhysicsEngine::Vector3D minPos(-5, 5, -5);
        const static PhysicsEngine::Vector3D maxPos(5, 10, 5);
        const static PhysicsEngine::Vector3D minSize(0.5, 0.5, 0.5);
        const static PhysicsEngine::Vector3D maxSize(4.5, 1.5, 1.5);

        setState(
            random->randomVector(minPos, maxPos),
            random->randomQuaternion(),
            random->randomVector(minSize, maxSize),
            PhysicsEngine::Vector3D()
            );
    }
};

/**
 * The main demo class definition.
 */
class Explosion : public RigidBodyApplication
{
    bool editMode, upMode;

    /**
     * Holds the number of boxes in the simulation.
     */
    const static unsigned boxes = OBJECTS;

    /** Holds the box data. */
    Box boxData[boxes];

    /**
     * Holds the number of balls in the simulation.
     */
    const static unsigned balls = OBJECTS;

    /** Holds the ball data. */
    Ball ballData[balls];


    /** Detonates the explosion. */
    void fire();

    /** Resets the position of all the boxes and primes the explosion. */
    virtual void reset();

    /** Processes the contact generation code. */
    virtual void generateContacts();

    /** Processes the objects in the simulation forward in time. */
    virtual void updateObjects(double dElapsedFrameTime);

public:
    /** Creates a new demo object. */
    Explosion();

    /** Sets up the rendering. */
    virtual void initGraphics();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Display the particle positions. */
    virtual void display();

    /** Handles a key press. */
    virtual void key(unsigned char key);

    /** Handle a mouse drag */
    virtual void mouseDrag(int x, int y);
};

// Method definitions
Explosion::Explosion()
    :
    RigidBodyApplication(),
    editMode(false),
    upMode(false)
{
    // Reset the position of the boxes
    reset();
}

const char* Explosion::getTitle()
{
    return "PhysicsEngine><RABBID>< Explosion Demo";
}

void Explosion::fire()
{
    PhysicsEngine::Vector3D pos = ballData[0].pBody->getBodyPosition();
    pos.normalize();

    ballData[0].pBody->addForceToObject(pos * -1000.0);
}

void Explosion::reset()
{
    Box *box = boxData;

    box++->setState(PhysicsEngine::Vector3D(0,3,0),
                    PhysicsEngine::Quaternion(),
                    PhysicsEngine::Vector3D(4,1,1),
                    PhysicsEngine::Vector3D(0,1,0));

    if (boxes > 1)
    {
        box++->setState(PhysicsEngine::Vector3D(0,4.75,2),
                        PhysicsEngine::Quaternion(1.0,0.1,0.05,0.01),
                        PhysicsEngine::Vector3D(1,1,4),
                        PhysicsEngine::Vector3D(0,1,0));
    }

    // Create the random objects
    PhysicsEngine::Random random;
    for (; box < boxData+boxes; box++)
    {
        box->random(&random);
    }

    for (Ball *ball = ballData; ball < ballData+balls; ball++)
    {
        ball->random(&random);
    }

    // Reset the contacts
    cData.wNumberOfContacts = 0;
}

void Explosion::generateContacts()
{
    // Note that this method makes a lot of use of early returns to avoid
    // processing lots of potential contacts that it hasn't got room to
    // store.

    // Create the ground plane data
    PhysicsEngine::CollisionPlane plane;
    plane.m_v3direction = PhysicsEngine::Vector3D(0,1,0);
    plane.m_dPrimitiveOffsetPlane = 0;

    // Set up the collision data structure
    cData.resetContacts(maxContacts);
    cData.m_dFriction = (double)0.9;
    cData.m_dRestitution = (double)0.6;
    cData.dCollisionTolerance = (double)0.1;

    // Perform exhaustive collision detection
    PhysicsEngine::Matrix4 transform, otherTransform;
    PhysicsEngine::Vector3D position, otherPosition;
    for (Box *box = boxData; box < boxData+boxes; box++)
    {
        // Check for collisions with the ground plane
        if (!cData.hasAnyMoreContacts()) return;
        PhysicsEngine::CollisionDetector::boxAndHalfSpace(*box, plane, &cData);

        // Check for collisions with each other box
        for (Box *other = box+1; other < boxData+boxes; other++)
        {
            if (!cData.hasAnyMoreContacts()) return;
            PhysicsEngine::CollisionDetector::boxAndBox(*box, *other, &cData);

            if (PhysicsEngine::QuickIntersectionTest::boxAndBoxIntersection(*box, *other))
            {
                box->isOverlapping = other->isOverlapping = true;
            }
        }

        // Check for collisions with each ball
        for (Ball *other = ballData; other < ballData+balls; other++)
        {
            if (!cData.hasAnyMoreContacts()) return;
            PhysicsEngine::CollisionDetector::boxAndSphere(*box, *other, &cData);
        }
    }

    for (Ball *ball = ballData; ball < ballData+balls; ball++)
    {
        // Check for collisions with the ground plane
        if (!cData.hasAnyMoreContacts()) return;
        PhysicsEngine::CollisionDetector::sphereAndHalfSpace(*ball, plane, &cData);

        for (Ball *other = ball+1; other < ballData+balls; other++)
        {
            // Check for collisions with the ground plane
            if (!cData.hasAnyMoreContacts()) return;
            PhysicsEngine::CollisionDetector::sphereAndSphere(*ball, *other, &cData);
        }
    }
}

void Explosion::updateObjects(double dElapsedFrameTime)
{
    // Update the physics of each box in turn
    for (Box *box = boxData; box < boxData+boxes; box++)
    {
        // Run the physics
        box->pBody->physicsIntegration(dElapsedFrameTime);
        box->calculatePrimitiveInternals();
        box->isOverlapping = false;
    }

    // Update the physics of each ball in turn
    for (Ball *ball = ballData; ball < ballData+balls; ball++)
    {
        // Run the physics
        ball->pBody->physicsIntegration(dElapsedFrameTime);
        ball->calculatePrimitiveInternals();
    }
}


void Explosion::initGraphics()
{
    GLfloat lightAmbient[] = {0.8f,0.8f,0.8f,1.0f};
    GLfloat lightDiffuse[] = {0.9f,0.95f,1.0f,1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glEnable(GL_LIGHT0);

    Application::initGraphics();
}

void Explosion::display()
{
    const static GLfloat lightPosition[] = {1,-1,0,0};
    const static GLfloat lightPositionMirror[] = {1,1,0,0};

    // Update the transform matrices of each box in turn
    for (Box *box = boxData; box < boxData+boxes; box++)
    {
        box->calculatePrimitiveInternals();
        box->isOverlapping = false;
    }

    // Update the transform matrices of each ball in turn
    for (Ball *ball = ballData; ball < ballData+balls; ball++)
    {
        // Run the physics
        ball->calculatePrimitiveInternals();
    }

    // Clear the viewport and set the camera direction
    RigidBodyApplication::display();

    // Render each element in turn as a shadow
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glPushMatrix();
    glMultMatrixf(floorMirror);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    for (Box *box = boxData; box < boxData+boxes; box++)
    {
        box->render();
    }
    for (Ball *ball = ballData; ball < ballData+balls; ball++)
    {
        ball->render();
    }
    glPopMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    // Draw some scale circles
    glColor3f(0.75, 0.75, 0.75);
    for (unsigned i = 1; i < 20; i++)
    {
        glBegin(GL_LINE_LOOP);
        for (unsigned j = 0; j < 32; j++)
        {
            float theta = 3.1415926f * j / 16.0f;
            glVertex3f(i*cosf(theta),0.0f,i*sinf(theta));
        }
        glEnd();
    }
    glBegin(GL_LINES);
    glVertex3f(-20,0,0);
    glVertex3f(20,0,0);
    glVertex3f(0,0,-20);
    glVertex3f(0,0,20);
    glEnd();

    // Render each shadow in turn
    glEnable(GL_BLEND);
    glColor4f(0,0,0,0.1f);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (Box *box = boxData; box < boxData+boxes; box++)
    {
        box->renderShadow();
    }
    for (Ball *ball = ballData; ball < ballData+balls; ball++)
    {
        ball->renderShadow();
    }
    glDisable(GL_BLEND);

    // Render the boxes themselves
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPositionMirror);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    for (Box *box = boxData; box < boxData+boxes; box++)
    {
        box->render();
    }
    for (Ball *ball = ballData; ball < ballData+balls; ball++)
    {
        ball->render();
    }
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Finish the frame, rendering any additional information
    drawDebug();
}

void Explosion::mouseDrag(int x, int y)
{
    if (editMode)
    {
        boxData[0].pBody->setBodyPosition(boxData[0].pBody->getBodyPosition() +
            PhysicsEngine::Vector3D(
                (x-last_x) * 0.125f,
                0,
                (y-last_y) * 0.125f
                )
            );
        boxData[0].pBody->calculateObjectData();
    }
    else if (upMode)
    {
        boxData[0].pBody->setBodyPosition(boxData[0].pBody->getBodyPosition() +
            PhysicsEngine::Vector3D(
                0,
                (y-last_y) * 0.125f,
                0
                )
            );
        boxData[0].pBody->calculateObjectData();
    }
    else
    {
        RigidBodyApplication::mouseDrag(x, y);
    }

    // Remember the position
    last_x = x;
    last_y = y;
}


void Explosion::key(unsigned char key)
{
    switch(key)
    {
    case 'e': case 'E':
        editMode = !editMode;
        upMode = false;
        return;

    case 't': case 'T':
        upMode = !upMode;
        editMode = false;
        return;

    case 'w': case 'W':
        for (Box *box = boxData; box < boxData+boxes; box++)
            box->pBody->setObjectStatis();
        for (Ball *ball = ballData; ball < ballData+balls; ball++)
            ball->pBody->setObjectStatis();
        return;
    }

    RigidBodyApplication::key(key);
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
    return new Explosion();
}