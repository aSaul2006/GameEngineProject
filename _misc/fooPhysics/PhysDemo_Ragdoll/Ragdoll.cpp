/***************************************************************************************
*  Program Name:    Ragdoll.cpp                                                        *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 14, 2013                                                  *
*  Description:     This file contains the definition file for the default application *
*                   object needed to run PhysDemo_Ragdoll                              *
*                   - Showcases a different aspect of the Physics Engine: play Demo    *
*                             - Seperate Solution -                                    *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
/*
 * The ragdoll demo.
 */

#include "../gl/glut.h"
#include "../PhysicsEngine/physicsengine/PhysicsEngine.h"
#include "../Common Files/timing.h"
#include "../Common Files/PhysApp.h"

#include <stdio.h>

#define NUM_BONES 12
#define NUM_JOINTS 11

class Bone : public PhysicsEngine::CollisionBox
{
public:
    Bone()
    {
        pBody = new PhysicsEngine::PhysBody();
    }

    ~Bone()
    {
        delete pBody;
    }

    /**
     * We use a sphere to collide bone on bone to allow some limited
     * interpenetration.
     */
    PhysicsEngine::CollisionSphere getCollisionSphere() const
    {
        PhysicsEngine::CollisionSphere sphere;
        sphere.pBody = pBody;
        sphere.m_dSphereRadius = m_v3HalfSizeBox.x;
        sphere.m_m4PrimitiveOffset = PhysicsEngine::Matrix4();
        if (m_v3HalfSizeBox.y < sphere.m_dSphereRadius) sphere.m_dSphereRadius = m_v3HalfSizeBox.y;
        if (m_v3HalfSizeBox.z < sphere.m_dSphereRadius) sphere.m_dSphereRadius = m_v3HalfSizeBox.z;
        sphere.calculatePrimitiveInternals();
        return sphere;
    }

    /** Draws the bone. */
    void render()
    {
        // Get the OpenGL transformation
        GLdouble mat[16];
        pBody->getGLTransform(mat);

        if (pBody->getObjectStatis()) glColor3f(0.5f, 0.3f, 0.3f);
        else glColor3f(0.3f, 0.3f, 0.5f);

        glPushMatrix();
        glMultMatrixd(mat);
        glScaled(m_v3HalfSizeBox.x*2, m_v3HalfSizeBox.y*2, m_v3HalfSizeBox.z*2);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    /** Sets the bone to a specific location. */
    void setState(const PhysicsEngine::Vector3D &position,
                  const PhysicsEngine::Vector3D &extents)
    {
        pBody->setBodyPosition(position);
        pBody->setBodyOrientationQ(PhysicsEngine::Quaternion());
        pBody->setObjectVelocity(PhysicsEngine::Vector3D());
        pBody->setObjectRotation(PhysicsEngine::Vector3D());
        m_v3HalfSizeBox = extents;

        double mass = m_v3HalfSizeBox.x * m_v3HalfSizeBox.y * m_v3HalfSizeBox.z * 8.0f;
        pBody->setBodyMass(mass);

        PhysicsEngine::Matrix3 tensor;
        tensor.SetBlockInertiaTensor(m_v3HalfSizeBox, mass);
        pBody->setInertiaTensor(tensor);

        pBody->setObjectLinearDamping(0.95f);
        pBody->setObjectAngularDamping(0.8f);
        pBody->emptyAccumulators();
        pBody->setBodyAcceleration(PhysicsEngine::Vector3D::GRAVITY);

        pBody->setCanGoIntoStatis(false);
        pBody->setObjectStatis();

        pBody->calculateObjectData();
        calculatePrimitiveInternals();
    }

};

/**
 * The main demo class definition.
 */
class RagdollDemo : public RigidBodyApplication
{
    PhysicsEngine::Random random;

    /** Holds the bone bodies. */
    Bone bones[NUM_BONES];

    /** Holds the joints. */
    PhysicsEngine::Joint joints[NUM_JOINTS];

    /** Processes the contact generation code. */
    virtual void generateContacts();

    /** Processes the objects in the simulation forward in time. */
    virtual void updateObjects(double dElapsedFrameTime);

    /** Resets the position of all the bones. */
    virtual void reset();

public:
    /** Creates a new demo object. */
    RagdollDemo();

    /** Sets up the rendering. */
    virtual void initGraphics();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Display the particle positions. */
    virtual void display();
};

// Method definitions
RagdollDemo::RagdollDemo()
    :
    RigidBodyApplication()
{
    // Set up the bone hierarchy.

    // Right Knee
    joints[0].set(
        bones[0].pBody, PhysicsEngine::Vector3D(0, 1.07, 0),
        bones[1].pBody, PhysicsEngine::Vector3D(0, -1.07, 0),
        0.15f
        );

    // Left Knee
    joints[1].set(
        bones[2].pBody, PhysicsEngine::Vector3D(0, 1.07f, 0),
        bones[3].pBody, PhysicsEngine::Vector3D(0, -1.07f, 0),
        0.15f
        );

    // Right elbow
    joints[2].set(
        bones[9].pBody, PhysicsEngine::Vector3D(0, 0.96f, 0),
        bones[8].pBody, PhysicsEngine::Vector3D(0, -0.96f, 0),
        0.15f
        );

    // Left elbow
    joints[3].set(
        bones[11].pBody, PhysicsEngine::Vector3D(0, 0.96f, 0),
        bones[10].pBody, PhysicsEngine::Vector3D(0, -0.96f, 0),
        0.15f
        );

    // Stomach to Waist
    joints[4].set(
        bones[4].pBody, PhysicsEngine::Vector3D(0.054f, 0.50f, 0),
        bones[5].pBody, PhysicsEngine::Vector3D(-0.043f, -0.45f, 0),
        0.15f
        );

    joints[5].set(
        bones[5].pBody, PhysicsEngine::Vector3D(-0.043f, 0.411f, 0),
        bones[6].pBody, PhysicsEngine::Vector3D(0, -0.411f, 0),
        0.15f
        );

    joints[6].set(
        bones[6].pBody, PhysicsEngine::Vector3D(0, 0.521f, 0),
        bones[7].pBody, PhysicsEngine::Vector3D(0, -0.752f, 0),
        0.15f
        );

    // Right hip
    joints[7].set(
        bones[1].pBody, PhysicsEngine::Vector3D(0, 1.066f, 0),
        bones[4].pBody, PhysicsEngine::Vector3D(0, -0.458f, -0.5f),
        0.15f
        );

    // Left Hip
    joints[8].set(
        bones[3].pBody, PhysicsEngine::Vector3D(0, 1.066f, 0),
        bones[4].pBody, PhysicsEngine::Vector3D(0, -0.458f, 0.5f),
        0.105f
        );

    // Right shoulder
    joints[9].set(
        bones[6].pBody, PhysicsEngine::Vector3D(0, 0.367f, -0.8f),
        bones[8].pBody, PhysicsEngine::Vector3D(0, 0.888f, 0.32f),
        0.15f
        );

    // Left shoulder
    joints[10].set(
        bones[6].pBody, PhysicsEngine::Vector3D(0, 0.367f, 0.8f),
        bones[10].pBody, PhysicsEngine::Vector3D(0, 0.888f, -0.32f),
        0.15f
        );

    // Set up the initial positions
    reset();
}

const char* RagdollDemo::getTitle()
{
    return "PhysicsEngine><RABBID>< Ragdoll Demo";
}

void RagdollDemo::generateContacts()
{
    // Create the ground plane data
    PhysicsEngine::CollisionPlane plane;
    plane.m_v3direction = PhysicsEngine::Vector3D(0,1,0);
    plane.m_dPrimitiveOffsetPlane = 0;

    // Set up the collision data structure
    cData.resetContacts(maxContacts);
    cData.m_dFriction = (double)0.9;
    cData.m_dRestitution = (double)0.6;
    cData.dCollisionTolerance = (double)0.1;

    // Perform exhaustive collision detection on the ground plane
    PhysicsEngine::Matrix4 transform, otherTransform;
    PhysicsEngine::Vector3D position, otherPosition;
    for (Bone *bone = bones; bone < bones+NUM_BONES; bone++)
    {
        // Check for collisions with the ground plane
        if (!cData.hasAnyMoreContacts()) return;
        PhysicsEngine::CollisionDetector::boxAndHalfSpace(*bone, plane, &cData);

        PhysicsEngine::CollisionSphere boneSphere = bone->getCollisionSphere();

        // Check for collisions with each other box
        for (Bone *other = bone+1; other < bones+NUM_BONES; other++)
        {
            if (!cData.hasAnyMoreContacts()) return;

            PhysicsEngine::CollisionSphere otherSphere = other->getCollisionSphere();

            PhysicsEngine::CollisionDetector::sphereAndSphere(
                boneSphere,
                otherSphere,
                &cData
                );
        }
    }

    // Check for joint violation
    for (PhysicsEngine::Joint *joint = joints; joint < joints+NUM_JOINTS; joint++)
    {
        if (!cData.hasAnyMoreContacts()) return;
        unsigned added = joint->addContact(cData.contacts, cData.m_iContactsMax);
        cData.addContacts(added);
    }
}

void RagdollDemo::reset()
{
    bones[0].setState(
        PhysicsEngine::Vector3D(0, 0.993, -0.5),
        PhysicsEngine::Vector3D(0.301, 1.0, 0.234));
    bones[1].setState(
        PhysicsEngine::Vector3D(0, 3.159, -0.56),
        PhysicsEngine::Vector3D(0.301, 1.0, 0.234));
    bones[2].setState(
        PhysicsEngine::Vector3D(0, 0.993, 0.5),
        PhysicsEngine::Vector3D(0.301, 1.0, 0.234));
    bones[3].setState(
        PhysicsEngine::Vector3D(0, 3.15, 0.56),
        PhysicsEngine::Vector3D(0.301, 1.0, 0.234));
    bones[4].setState(
        PhysicsEngine::Vector3D(-0.054, 4.683, 0.013),
        PhysicsEngine::Vector3D(0.415, 0.392, 0.690));
    bones[5].setState(
        PhysicsEngine::Vector3D(0.043, 5.603, 0.013),
        PhysicsEngine::Vector3D(0.301, 0.367, 0.693));
    bones[6].setState(
        PhysicsEngine::Vector3D(0, 6.485, 0.013),
        PhysicsEngine::Vector3D(0.435, 0.367, 0.786));
    bones[7].setState(
        PhysicsEngine::Vector3D(0, 7.759, 0.013),
        PhysicsEngine::Vector3D(0.45, 0.598, 0.421));
    bones[8].setState(
        PhysicsEngine::Vector3D(0, 5.946, -1.066),
        PhysicsEngine::Vector3D(0.267, 0.888, 0.207));
    bones[9].setState(
        PhysicsEngine::Vector3D(0, 4.024, -1.066),
        PhysicsEngine::Vector3D(0.267, 0.888, 0.207));
    bones[10].setState(
        PhysicsEngine::Vector3D(0, 5.946, 1.066),
        PhysicsEngine::Vector3D(0.267, 0.888, 0.207));
    bones[11].setState(
        PhysicsEngine::Vector3D(0, 4.024, 1.066),
        PhysicsEngine::Vector3D(0.267, 0.888, 0.207));

    double strength = -random.randomDouble(500.0f, 1000.0f);
    for (unsigned i = 0; i < NUM_BONES; i++)
    {
        bones[i].pBody->addForceAtBodyPoint(
            PhysicsEngine::Vector3D(strength, 0, 0), PhysicsEngine::Vector3D()
            );
    }
    bones[6].pBody->addForceAtBodyPoint(
        PhysicsEngine::Vector3D(strength, 0, random.randomBinomial(1000.0f)),
        PhysicsEngine::Vector3D(random.randomBinomial(4.0f), random.randomBinomial(3.0f), 0)
        );

    // Reset the contacts
    cData.wNumberOfContacts = 0;
}

void RagdollDemo::updateObjects(double dElapsedFrameTime)
{
    for (Bone *bone = bones; bone < bones+NUM_BONES; bone++)
    {
        bone->pBody->physicsIntegration(dElapsedFrameTime);
        bone->calculatePrimitiveInternals();
    }
}

void RagdollDemo::initGraphics()
{
    GLfloat lightAmbient[] = {0.8f,0.8f,0.8f,1.0f};
    GLfloat lightDiffuse[] = {0.9f,0.95f,1.0f,1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glEnable(GL_LIGHT0);

    Application::initGraphics();
}

void RagdollDemo::display()
{
    const static GLfloat lightPosition[] = {0.7f,-1,0.4f,0};
    const static GLfloat lightPositionMirror[] = {0.7f,1,0.4f,0};

    RigidBodyApplication::display();

    // Render the bones
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_NORMALIZE);
    glColor3f(1,0,0);
    for (unsigned i = 0; i < NUM_BONES; i++)
    {
        bones[i].render();
    }
    glDisable(GL_NORMALIZE);

    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    glDisable(GL_DEPTH_TEST);
    glBegin(GL_LINES);
    for (unsigned i = 0; i < NUM_JOINTS; i++)
    {
        PhysicsEngine::Joint *joint = joints + i;
        PhysicsEngine::Vector3D a_pos = joint->body[0]->getPointInWorldSpace(joint->position[0]);
        PhysicsEngine::Vector3D b_pos = joint->body[1]->getPointInWorldSpace(joint->position[1]);
        double length = (b_pos - a_pos).magnitude();

        if (length > joint->error) glColor3f(1,0,0);
        else glColor3f(0,1,0);

        glVertex3f(a_pos.x, a_pos.y, a_pos.z);
        glVertex3f(b_pos.x, b_pos.y, b_pos.z);
    }
    glEnd();
    glEnable(GL_DEPTH_TEST);

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

    RigidBodyApplication::drawDebug();
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
    return new RagdollDemo();
}