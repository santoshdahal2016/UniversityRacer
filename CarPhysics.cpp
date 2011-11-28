#include "CarPhysics.h"


CarPhysics::CarPhysics(void): m_engineForce(0.f), m_breakingForce(0.f), m_vehicleSteering(0.f), turned(false),
                              m_lastCheckpointPos(0.f, 0.f, 0.f)
{                             
}    

CarPhysics::~CarPhysics(void)
{
}

btCollisionShape* CarPhysics::CreateVehicleUpperShape()
{
    static btScalar vertices[] = {
                     
        -0.9f, -0.2f, -1.5f,
        -0.9f, -0.2f,  1.5f,
        -0.9f,  0.2f,  0.5f,
        -0.9f,  0.2f, -1.0f,

        0.9f, -0.2f, -1.5f,
        0.9f, -0.2f,  1.5f,
        0.9f,  0.2f,  0.5f,
        0.9f,  0.2f, -1.0f
        
    };
    //btConvexHullShape gh; gh.
    return new btConvexHullShape(vertices, 8, 3*sizeof(btScalar));
}

btCollisionShape* CarPhysics::CreateVehicleShape()
{
   /* static btScalar vertices[] = {
                     
        -1.0f,  0.0f, -2.0f,
         1.0f,  0.0f, -2.0f,

        -1.0f,  0.5f, -3.0f,
         1.0f,  0.5f, -3.0f,

        -1.0f, -0.2f, -3.0f,
         1.0f, -0.2f, -3.0f,
         1.0f, -0.2f,  3.0f,
         1.0f,  0.2f,  3.0f,
        -1.0f, -0.2f,  3.0f,
        -1.0f,  0.2f,  3.0f
    };*/

    static btScalar vertices[] = {
                     
        -1.0f,  0.0f, -2.5f,
        -1.0f, -0.3f, -2.4f,
        -1.0f, -0.6f, -2.0f,
        -1.0f, -0.6f,  2.0f,
        -1.0f, -0.2f,  2.4f,
        -1.0f,  0.1f,  2.5f,
        -1.0f,  0.2f,  2.4f,
        -1.0f,  0.6f,  1.0f,
        -1.0f,  0.6f, -2.0f, 
        -1.0f,  0.5f, -2.4f,     

         1.0f,  0.0f, -2.5f,
         1.0f, -0.3f, -2.4f,
         1.0f, -0.6f, -2.0f,
         1.0f, -0.6f,  2.0f,
         1.0f, -0.2f,  2.4f,
         1.0f,  0.1f,  2.5f,
         1.0f,  0.2f,  2.4f,
         1.0f,  0.6f,  1.0f,
         1.0f,  0.6f, -2.0f, 
         1.0f,  0.5f, -2.4f,
    };

    return new btConvexHullShape(vertices, 20, 3*sizeof(btScalar));
    
    
    /*static btScalar vertices[8*3] = {
        -1.0, -1.0, -1.0,
        -1.0,  1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0, -1.0,  1.0,
        -1.0,  1.0,  1.0
    };

    static int indices[12*3] = {
        0, 1, 3,
        2, 3, 5,
        4, 5, 7,
        6, 7, 1,
        1, 7, 5,
        6, 0, 2,
        0, 3, 2,
        2, 5, 4,
        4, 7, 6,
        6, 1, 0,
        1, 5, 3,
        6, 2, 4
    };

  btConvexHullShape *cvs = new btConvexHullShape();
  btTriangleMesh *trimesh = new btTriangleMesh();
  for (int i = 0; i < 12; i++)
  {
     int index0 = indices[i * 3];
     int index1 = indices[i * 3 + 1];
     int index2 = indices[i * 3 + 2];

     btVector3 vertex0(vertices[index0], vertices[index0+1], vertices[index0+2]);
     btVector3 vertex1(vertices[index1], vertices[index1+1], vertices[index1+2]);
     btVector3 vertex2(vertices[index2], vertices[index2+1], vertices[index2+2]);

     trimesh->addTriangle(vertex0, vertex1, vertex2);
    //cvs->addPoint(vertex0);
    //cvs->addPoint(vertex1);
    //cvs->addPoint(vertex2);
  }*/
 
/* btConvexShape *tmpshape = new btConvexTriangleMeshShape(trimesh);
  btShapeHull *hull = new btShapeHull(tmpshape);
  btScalar margin = tmpshape->getMargin();
  hull->buildHull(margin);*/
  //tmpshape->setUserPointer(hull);
  //return new btConvexHullShape((btScalar*) &(hull->getVertexPointer())[0].x(),hull->numVertices());//, 3*sizeof(btScalar));

     //btTriangleIndexVertexArray* indexVertexArrays = new btTriangleIndexVertexArray(
  //      12, indices, 3*sizeof(int), 8, vertices, 3*sizeof(btScalar));
  //  btConvexTriangleMeshShape *ms = new btConvexTriangleMeshShape(indexVertexArrays);
    

    //create a hull approximation
  //  btShapeHull *hull = new btShapeHull(ms);
  //  btScalar margin = ms->getMargin();
  //  hull->buildHull(margin);
 //   return new btConvexHullShape((btScalar*) &(ms->getConvexPolyhedron()->m_vertices[0].x()),ms->getConvexPolyhedron()->m_vertices.size());

    //btConvexHullShape *ch = new btConvexHullShape(vertices, 8, 3*sizeof(btScalar));*/
}

void CarPhysics::Initialize(btDiscreteDynamicsWorld *refWorld)
{
    m_refDynamicsWorld = refWorld;

    btCollisionShape* chassisShape = CreateVehicleShape();//new btBoxShape(btVector3(1.f, 0.5f, 2.f));
    m_collisionShapes.push_back(chassisShape);

    btCompoundShape* compound = new btCompoundShape();
    m_collisionShapes.push_back(compound);
    
    btTransform localTrans;
    localTrans.setIdentity();

    btCollisionShape* chassisShape2 = CreateVehicleUpperShape();//CreateVehicleShape(); //new btBoxShape(btVector3(0.7f, 0.2f, 1.7f));
    localTrans.setOrigin(btVector3(0,1.8f,-0.5f));  //1.7 localTrans effectively shifts the center of mass with respect to the chassis
    compound->addChildShape(localTrans, chassisShape2);

    localTrans.setOrigin(btVector3(0.f, 1.0f, 0.f));  //1.5localTrans effectively shifts the center of mass with respect to the chassis
    compound->addChildShape(localTrans, chassisShape);

    
    
    btTransform tr;
    tr.setIdentity();
    //tr.setOrigin(btVector3(0,10,0));  
    tr.setRotation(btQuaternion(btVector3(0.f, 1.f, 0.f), 3.14f));

    
    btVector3 localInertia(0, 0, 0);
    compound->calculateLocalInertia(m_cfg.mass, localInertia);
    btDefaultMotionState* myMotionState = new btDefaultMotionState(tr);
    btRigidBody::btRigidBodyConstructionInfo cInfo(m_cfg.mass, myMotionState, compound,localInertia);
    m_carChassis = new btRigidBody(cInfo);
    m_refDynamicsWorld->addRigidBody(m_carChassis);
    //m_carChassis->setDamping(0.2f,0.2f);
    

    /// create vehicle            
    
    m_vehicleRayCaster = new btDefaultVehicleRaycaster(m_refDynamicsWorld);
    
    btRaycastVehicle::btVehicleTuning  m_tuning;
    //m_tuning.
    m_vehicle = new btRaycastVehicle(m_tuning, m_carChassis, m_vehicleRayCaster);
    m_refDynamicsWorld->addVehicle(m_vehicle);        
    
    m_carChassis->setActivationState(DISABLE_DEACTIVATION);  ///never deactivate the vehicle
    m_vehicle->setCoordinateSystem(0, 1, 2);  //choose coordinate system

            
    // add wheels
    btVector3 connectionPointCS0;

    // front left
    connectionPointCS0 = btVector3( m_cfg.connectionWidth, m_cfg.connectionHeight,  m_cfg.connectionLength);
    m_vehicle->addWheel(connectionPointCS0, m_cfg.wheelDirectionCS0, m_cfg.wheelAxleCS, m_cfg.suspensionRestLength, m_cfg.wheelRadius, m_tuning, true);

    // front right
    connectionPointCS0 = btVector3(-m_cfg.connectionWidth, m_cfg.connectionHeight,  m_cfg.connectionLength);
    m_vehicle->addWheel(connectionPointCS0, m_cfg.wheelDirectionCS0, m_cfg.wheelAxleCS, m_cfg.suspensionRestLength, m_cfg.wheelRadius, m_tuning, true);

    // back right
    connectionPointCS0 = btVector3(-m_cfg.connectionWidth, m_cfg.connectionHeight, -m_cfg.connectionLength);
    m_vehicle->addWheel(connectionPointCS0, m_cfg.wheelDirectionCS0, m_cfg.wheelAxleCS, m_cfg.suspensionRestLength, m_cfg.wheelRadius, m_tuning, false);

    // back left
    connectionPointCS0 = btVector3( m_cfg.connectionWidth, m_cfg.connectionHeight, -m_cfg.connectionLength);
    m_vehicle->addWheel(connectionPointCS0, m_cfg.wheelDirectionCS0, m_cfg.wheelAxleCS, m_cfg.suspensionRestLength, m_cfg.wheelRadius, m_tuning, false);
    
    for (int i = 0; i < m_vehicle->getNumWheels(); i++)
    {
        btWheelInfo& wheel = m_vehicle->getWheelInfo(i);

        wheel.m_suspensionStiffness = m_cfg.suspensionStiffness;
        wheel.m_wheelsDampingRelaxation = m_cfg.suspensionDamping;
        wheel.m_wheelsDampingCompression = m_cfg.suspensionCompression;
        wheel.m_frictionSlip = m_cfg.wheelFriction;
        wheel.m_rollInfluence = m_cfg.rollInfluence;
        //wheel.m_maxSuspensionTravelCm
    }
}

void CarPhysics::Deinitialize()
{




}

void CarPhysics::Reset()
{
    m_vehicleSteering = 0.f;
    
    btTransform tr;
    tr.setIdentity();
    tr.setRotation(btQuaternion(btVector3(0.f, 1.f, 0.f), 3.14f));
    tr.setOrigin(m_lastCheckpointPos);
    m_carChassis->setCenterOfMassTransform(tr);
    m_carChassis->setLinearVelocity(btVector3(0.f, 0.f, 0.f));
    m_carChassis->setAngularVelocity(btVector3(0.f, 0.f, 0.f));
    

    m_refDynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(), m_refDynamicsWorld->getDispatcher());
    if (m_vehicle)
    {
        m_vehicle->resetSuspension();
        for (int i = 0; i < m_vehicle->getNumWheels(); i++)
            m_vehicle->updateWheelTransform(i, true); //synchronize the wheels with the (interpolated) chassis worldtransform
    }
}

void CarPhysics::Update(btScalar timeStep)
{
    btScalar speed = std::abs(m_vehicle->getCurrentSpeedKmHour());
       
    if (speed > 100.f)
    {
        m_cfg.steeringClamp = 0.1f;
        m_cfg.steeringIncrement = 0.004f;
    }
    else
    {
        m_cfg.steeringClamp = 0.1f + 0.65f - ((0.65f * speed) / 100.f);
        m_cfg.steeringIncrement = 0.04f - ((0.004f * speed) / 100.f);
    }        
    
    
    m_vehicle->applyEngineForce(m_engineForce, 2);
    m_vehicle->setBrake(m_breakingForce, 2);
    m_vehicle->applyEngineForce(m_engineForce, 3);
    m_vehicle->setBrake(m_breakingForce, 3);

    
    m_vehicle->setSteeringValue(m_vehicleSteering, 0);
    m_vehicle->setSteeringValue(m_vehicleSteering, 1);

    m_engineForce = 0.f;
    m_breakingForce = 0.f;
    
    if (!turned)
    {
        if (m_vehicleSteering > 0.f)
        {
            m_vehicleSteering -= m_cfg.steeringIncrement;
            if (m_vehicleSteering <= 0.f)
                m_vehicleSteering = 0.f;
        }
        else
        {
            m_vehicleSteering += m_cfg.steeringIncrement;
            if (m_vehicleSteering >= 0.f)
                m_vehicleSteering = 0.f;
        }
    }
    turned = false;
}

void CarPhysics::TurnLeft()
{
    m_vehicleSteering += m_cfg.steeringIncrement;
    if (m_vehicleSteering > m_cfg.steeringClamp)
        m_vehicleSteering = m_cfg.steeringClamp;    
    turned = true;
}

void CarPhysics::TurnRight()
{
    m_vehicleSteering -= m_cfg.steeringIncrement;
    if (m_vehicleSteering < -m_cfg.steeringClamp)
        m_vehicleSteering = -m_cfg.steeringClamp;  
    turned = true;
}

void CarPhysics::Forward()
{
    m_engineForce = m_cfg.maxEngineForce;
    m_breakingForce = 0.f;    
}

void CarPhysics::Backward()
{
    m_engineForce = -m_cfg.maxEngineForce;
    m_breakingForce = 0.f;    
}

void CarPhysics::HandBrake()
{   
    m_breakingForce = m_cfg.maxBreakingForce; 
    m_engineForce = 0.f;    
}