/***************************************************************************************
*  Program Name:    ProjectilesExpanded.cpp                                            *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 14, 2013                                                  *
*  Description:     This file contains the definition file for the default application *
*                   object needed to run PhysDemo_ProjectilesExpanded                  *
*                   - Showcases a different aspect of the Physics Engine: play Demo    *
*                             - Seperate Solution -                                    *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * The Expanded <interact with Boxes> Projectiles demo.
 */

#include "../gl/glut.h"
#include "../PhysicsEngine/physicsengine/PhysicsEngine.h"
#include "../Common Files/timing.h"
#include "../Common Files/PhysApp.h"

#include <stdio.h>

enum ShotType
{
    UNUSED = 0,
    PISTOL,
    ARTILLERY,
    FIREBALL,
    LASER
};

class AmmoRound : public PhysicsEngine::CollisionSphere
{
public:
    ShotType type;
    unsigned startTime;

    AmmoRound()
    {
        pBody = new PhysicsEngine::PhysBody;
    }

    ~AmmoRound()
    {
        delete pBody;
    }

    /** Draws the box, excluding its shadow. */
    void render()
    {
        // Get the OpenGL transformation
        GLdouble mat[16];
        pBody->getGLTransform(mat);

        glPushMatrix();
        glMultMatrixd(mat);
        glutSolidSphere(m_dSphereRadius, 20, 20);
        glPopMatrix();
    }

    /** Sets the box to a specific location. */
    void setState(ShotType shotType)
    {
        type = shotType;

        // Set the properties of the particle
        switch(type)
        {
        case PISTOL:
            pBody->setBodyMass(1.5f);
            pBody->setObjectVelocity(0.0f, 0.0f, 20.0f);
            pBody->setBodyAcceleration(0.0f, -0.5f, 0.0f);
            pBody->setObjectDamping(0.99f, 0.8f);
            m_dSphereRadius = 0.2f;
            break;

        case ARTILLERY:
            pBody->setBodyMass(200.0f); // 200.0kg
            pBody->setObjectVelocity(0.0f, 30.0f, 40.0f); // 50m/s
            pBody->setBodyAcceleration(0.0f, -21.0f, 0.0f);
            pBody->setObjectDamping(0.99f, 0.8f);
            m_dSphereRadius = 0.4f;
            break;

        case FIREBALL:
            pBody->setBodyMass(4.0f); // 4.0kg - mostly blast damage
            pBody->setObjectVelocity(0.0f, -0.5f, 10.0); // 10m/s
            pBody->setBodyAcceleration(0.0f, 0.3f, 0.0f); // Floats up
            pBody->setObjectDamping(0.9f, 0.8f);
            m_dSphereRadius = 0.6f;
            break;

        case LASER:
            // Note that this is the kind of laser bolt seen in films,
            // not a realistic laser beam!
            pBody->setBodyMass(0.1f); // 0.1kg - almost no weight
            pBody->setObjectVelocity(0.0f, 0.0f, 100.0f); // 100m/s
            pBody->setBodyAcceleration(0.0f, 0.0f, 0.0f); // No gravity
            pBody->setObjectDamping(0.99f, 0.8f);
            m_dSphereRadius = 0.2f;
            break;
        }

        pBody->setCanGoIntoStatis(false);
        pBody->setObjectStatis();

        PhysicsEngine::Matrix3 tensor;
        double coeff = 0.4f*pBody->getBodyMass()*m_dSphereRadius*m_dSphereRadius;
        tensor.SetInertiaTensorCoeffs(coeff,coeff,coeff);
        pBody->setInertiaTensor(tensor);

        // Set the data common to all particle types
        pBody->setBodyPosition(0.0f, 1.5f, 0.0f);
        startTime = TimingData::get().lastFrameTimestamp;

        // Clear the force accumulators
        pBody->calculateObjectData();
        calculatePrimitiveInternals();
    }
};

class Box : public PhysicsEngine::CollisionBox
{
public:
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

        glPushMatrix();
        glMultMatrixd(mat);
        glScalef(m_v3HalfSizeBox.x*2, m_v3HalfSizeBox.y*2, m_v3HalfSizeBox.z*2);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    /** Sets the box to a specific location. */
    void setState(double z)
    {
        pBody->setBodyPosition(0, 3, z);
        pBody->setBodyOrientation(1,0,0,0);
        pBody->setObjectVelocity(0,0,0);
        pBody->setObjectRotation(PhysicsEngine::Vector3D(0,0,0));
        m_v3HalfSizeBox = PhysicsEngine::Vector3D(1,1,1);

        double mass = m_v3HalfSizeBox.x * m_v3HalfSizeBox.y * m_v3HalfSizeBox.z * 8.0f;
        pBody->setBodyMass(mass);

        PhysicsEngine::Matrix3 tensor;
        tensor.SetBlockInertiaTensor(m_v3HalfSizeBox, mass);
        pBody->setInertiaTensor(tensor);

        pBody->setObjectLinearDamping(0.95f);
        pBody->setObjectAngularDamping(0.8f);
        pBody->emptyAccumulators();
        pBody->setBodyAcceleration(0,-10.0f,0);

        pBody->setCanGoIntoStatis(false);
        pBody->setObjectStatis();

        pBody->calculateObjectData();
        calculatePrimitiveInternals();
    }
};


/**
 * The main demo class definition.
 */
class ProjectilesExpanded : public RigidBodyApplication
{
    /**
     * Holds the maximum number of  rounds that can be
     * fired.
     */
    const static unsigned ammoRounds = 256;

    /** Holds the particle data. */
    AmmoRound ammo[ammoRounds];

    /**
    * Holds the number of boxes in the simulation.
    */
    const static unsigned boxes = 2;

    /** Holds the box data. */
    Box boxData[boxes];

    /** Holds the current shot type. */
    ShotType currentShotType;

    /** Resets the position of all the boxes and primes the explosion. */
    virtual void reset();

    /** Build the contacts for the current situation. */
    virtual void generateContacts();

    /** Processes the objects in the simulation forward in time. */
    virtual void updateObjects(double dElapsedFrameTime);

    /** Dispatches a round. */
    void fire();

public:
    /** Creates a new demo object. */
    ProjectilesExpanded();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Sets up the rendering. */
    virtual void initGraphics();
    
    /** Display world. */
    virtual void display();

    /** Handle a mouse click. */
    virtual void mouse(int button, int state, int x, int y);

    /** Handle a keypress. */
    virtual void key(unsigned char key);
};

// Method definitions
ProjectilesExpanded::ProjectilesExpanded()
: 
RigidBodyApplication(),
currentShotType(LASER)
{
    pauseSimulation = false;
    reset();
}


void ProjectilesExpanded::initGraphics()
{
    GLfloat lightAmbient[] = {0.8f,0.8f,0.8f,1.0f};
    GLfloat lightDiffuse[] = {0.9f,0.95f,1.0f,1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glEnable(GL_LIGHT0);

    Application::initGraphics();
}

void ProjectilesExpanded::reset()
{
    // Make all shots unused
    for (AmmoRound *shot = ammo; shot < ammo+ammoRounds; shot++)
    {
        shot->type = UNUSED;
    }

    // Initialise the box
    double z = 20.0;
    for (Box *box = boxData; box < boxData+boxes; box++)
    {
        box->setState(z);
        z += 90.0;
    }
}

const char* ProjectilesExpanded::getTitle()
{
    return "PhysicsEngine><RABBID>< Expanded Projectiles Demo";
}

void ProjectilesExpanded::fire()
{
    // Find the first available round.
    AmmoRound *shot;
    for (shot = ammo; shot < ammo+ammoRounds; shot++)
    {
        if (shot->type == UNUSED) break;
    }

    // If we didn't find a round, then exit - we can't fire.
    if (shot >= ammo+ammoRounds) return;

    // Set the shot
    shot->setState(currentShotType);

}

void ProjectilesExpanded::updateObjects(double dElapsedFrameTime)
{
    // Update the physics of each particle in turn
    for (AmmoRound *shot = ammo; shot < ammo+ammoRounds; shot++)
    {
        if (shot->type != UNUSED)
        {
            // Run the physics
            shot->pBody->physicsIntegration(dElapsedFrameTime);
            shot->calculatePrimitiveInternals();

            // Check if the particle is now invalid
            if (shot->pBody->getBodyPosition().y < 0.0f ||
                shot->startTime+5000 < TimingData::get().lastFrameTimestamp ||
                shot->pBody->getBodyPosition().z > 200.0f)
            {
                // We simply set the shot type to be unused, so the
                // memory it occupies can be reused by another shot.
                shot->type = UNUSED;
            }
        }
    }

    // Update the boxes
    for (Box *box = boxData; box < boxData+boxes; box++)
    {
        // Run the physics
        box->calculatePrimitiveInternals();
    }
}

void ProjectilesExpanded::display()
{
    const static GLfloat lightPosition[] = {-1,1,0,0};
    
    // Clear the viewport and set the camera direction
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(-25.0, 8.0, 5.0,  0.0, 5.0, 22.0,  0.0, 1.0, 0.0);

    // Draw a sphere at the firing point, and add a shadow projected
    // onto the ground plane.
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glutSolidSphere(0.1f, 5, 5);
    glTranslatef(0.0f, -1.5f, 0.0f);
    glColor3f(0.75f, 0.75f, 0.75f);
    glScalef(1.0f, 0.1f, 1.0f);
    glutSolidSphere(0.1f, 5, 5);
    glPopMatrix();

    // Draw some scale lines
    glColor3f(0.75f, 0.75f, 0.75f);
    glBegin(GL_LINES);
    for (unsigned i = 0; i < 200; i += 10)
    {
        glVertex3f(-5.0f, 0.0f, i);
        glVertex3f(5.0f, 0.0f, i);
    }
    glEnd();

    // Render each particle in turn
    glColor3f(1,0,0);
    for (AmmoRound *shot = ammo; shot < ammo+ammoRounds; shot++)
    {
        if (shot->type != UNUSED)
        {
            shot->render();
        }
    }

    // Render the box
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1,0,0);
    for (Box *box = boxData; box < boxData+boxes; box++)
    {
        box->render();
    }
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Render the description
    glColor3f(0.0f, 0.0f, 0.0f);
    renderText(10.0f, 34.0f, "Click: Fire\n1-4: Select Ammo");

    // Render the name of the current shot type
    switch(currentShotType)
    {
    case PISTOL: renderText(10.0f, 10.0f, "Current Ammo: Pistol"); break;
    case ARTILLERY: renderText(10.0f, 10.0f, "Current Ammo: Artillery"); break;
    case FIREBALL: renderText(10.0f, 10.0f, "Current Ammo: Fireball"); break;
    case LASER: renderText(10.0f, 10.0f, "Current Ammo: Laser"); break;
    }
}

void ProjectilesExpanded::generateContacts()
{
    // Create the ground plane data
    PhysicsEngine::CollisionPlane plane;
    plane.m_v3direction = PhysicsEngine::Vector3D(0,1,0);
    plane.m_dPrimitiveOffsetPlane = 0;

    // Set up the collision data structure
    cData.resetContacts(maxContacts);
    cData.m_dFriction = (double)0.9;
    cData.m_dRestitution = (double)0.1;
    cData.dCollisionTolerance = (double)0.1;

    // Check ground plane collisions
    for (Box *box = boxData; box < boxData+boxes; box++)
    {
        if (!cData.hasAnyMoreContacts()) return;
        PhysicsEngine::CollisionDetector::boxAndHalfSpace(*box, plane, &cData);


        // Check for collisions with each shot
        for (AmmoRound *shot = ammo; shot < ammo+ammoRounds; shot++)
        {
            if (shot->type != UNUSED)
            {
                if (!cData.hasAnyMoreContacts()) return;

                // When we get a collision, remove the shot
                if (PhysicsEngine::CollisionDetector::boxAndSphere(*box, *shot, &cData))
                {
                    shot->type = UNUSED;
                }
            }
        }
    }

    // NB We aren't checking box-box collisions.
}

void ProjectilesExpanded::mouse(int button, int state, int x, int y)
{
    // Fire the current weapon.
    if (state == GLUT_DOWN) fire();
}

void ProjectilesExpanded::key(unsigned char key)
{
    switch(key)
    {
    case '1': currentShotType = PISTOL; break;
    case '2': currentShotType = ARTILLERY; break;
    case '3': currentShotType = FIREBALL; break;
    case '4': currentShotType = LASER; break;

    case 'r': case 'R': reset(); break;
    }
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
    return new ProjectilesExpanded();
}