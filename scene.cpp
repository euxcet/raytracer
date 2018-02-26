#include "common.h"
#include "scene.h"

namespace Raytracer {


void Scene::Init() {
	primitive = new Primitive*[500];
	// ground plane
	primitive[0] = new PlanePrim( vec3( 0, 1, 0 ), 4.4f );
	primitive[0]->SetName( "plane" );
	primitive[0]->GetMaterial()->SetReflection( 0.0f );
	primitive[0]->GetMaterial()->SetRefraction( 0.0f );
	primitive[0]->GetMaterial()->SetDiffuse( 1.0f );
	primitive[0]->GetMaterial()->SetColor( Color( 0.4f, 0.3f, 0.3f ) );
	// big sphere
	primitive[1] = new Sphere( vec3( 2, 0.8f, 3 ), 2.5f );
	primitive[1]->SetName( "big sphere" );
	primitive[1]->GetMaterial()->SetReflection( 0.2f );
	primitive[1]->GetMaterial()->SetRefraction( 0.8f );
	primitive[1]->GetMaterial()->SetRefrIndex( 1.3f );
	primitive[1]->GetMaterial()->SetColor( Color( 0.7f, 0.7f, 1.0f ) );
	// small sphere
	primitive[2] = new Sphere( vec3( -5.5f, -0.5, 7 ), 2 );
	primitive[2]->SetName( "small sphere" );
	primitive[2]->GetMaterial()->SetReflection( 0.5f );
	primitive[2]->GetMaterial()->SetRefraction( 0.0f );
	primitive[2]->GetMaterial()->SetRefrIndex( 1.3f );
	primitive[2]->GetMaterial()->SetDiffuse( 0.1f );
	primitive[2]->GetMaterial()->SetColor( Color( 0.7f, 0.7f, 1.0f ) );
	// light source 1
	primitive[3] = new Sphere( vec3( 0, 5, 5 ), 0.1f );
	primitive[3]->Light( true );
	primitive[3]->GetMaterial()->SetColor( Color( 0.4f, 0.4f, 0.4f ) );
	// light source 2
	primitive[4] = new Sphere( vec3( -3, 5, 1 ), 0.1f );
	primitive[4]->Light( true );
	primitive[4]->GetMaterial()->SetColor( Color( 0.6f, 0.6f, 0.8f ) );
	// extra sphere
	primitive[5] = new Sphere( vec3( -1.5f, -3.8f, 1 ), 1.5f );
	primitive[5]->SetName( "extra sphere" );
	primitive[5]->GetMaterial()->SetReflection( 0.0f );
	primitive[5]->GetMaterial()->SetRefraction( 0.8f );
	primitive[5]->GetMaterial()->SetColor( Color( 1.0f, 0.4f, 0.4f ) );
	// back plane
	primitive[6] = new PlanePrim( vec3( 0.4f, 0, -1 ), 12 );
	primitive[6]->SetName( "back plane" );
	primitive[6]->GetMaterial()->SetReflection( 0.0f );
	primitive[6]->GetMaterial()->SetRefraction( 0.0f );
	primitive[6]->GetMaterial()->SetSpecular( 0 );
	primitive[6]->GetMaterial()->SetDiffuse( 0.6f );
	primitive[6]->GetMaterial()->SetColor( Color( 0.5f, 0.3f, 0.5f ) );
	// ceiling plane
	primitive[7] = new PlanePrim( vec3( 0, -1, 0 ), 7.4f );
	primitive[7]->SetName( "back plane" );
	primitive[7]->GetMaterial()->SetReflection( 0.0f );
	primitive[7]->GetMaterial()->SetRefraction( 0.0f );
	primitive[7]->GetMaterial()->SetSpecular( 0 );
	primitive[7]->GetMaterial()->SetDiffuse( 0.5f );
	primitive[7]->GetMaterial()->SetColor( Color( 0.4f, 0.7f, 0.7f ) );
	// grid
	int prim = 8;
	for ( int x = 0; x < 8; x++ ) for ( int y = 0; y < 7; y++ )
	{
		primitive[prim] = new Sphere( vec3( -4.5f + x * 1.5f, -4.3f + y * 1.5f, 10 ), 0.3f );
		primitive[prim]->SetName( "grid sphere" );
		primitive[prim]->GetMaterial()->SetReflection( 0 );
		primitive[prim]->GetMaterial()->SetRefraction( 0 );
		primitive[prim]->GetMaterial()->SetSpecular( 0.6f );
		primitive[prim]->GetMaterial()->SetDiffuse( 0.6f );
		primitive[prim]->GetMaterial()->SetColor( Color( 0.3f, 1.0f, 0.4f ) );
		prim++;
	}
	for ( int x = 0; x < 8; x++ ) for ( int y = 0; y < 8; y++ )
	{
		primitive[prim] = new Sphere( vec3( -4.5f + x * 1.5f, -4.3f, 10.0f - y * 1.5f ), 0.3f );
		primitive[prim]->SetName( "grid sphere" );
		primitive[prim]->GetMaterial()->SetReflection( 0 );
		primitive[prim]->GetMaterial()->SetRefraction( 0 );
		primitive[prim]->GetMaterial()->SetSpecular( 0.6f );
		primitive[prim]->GetMaterial()->SetDiffuse( 0.6f );
		primitive[prim]->GetMaterial()->SetColor( Color( 0.3f, 1.0f, 0.4f ) );
		prim++;
	}
	for ( int x = 0; x < 16; x++ ) for ( int y = 0; y < 8; y++ )
	{
		primitive[prim] = new Sphere( vec3( -8.5f + x * 1.5f, 4.3f, 10.0f - y ), 0.3f );
		primitive[prim]->SetName( "grid sphere" );
		primitive[prim]->GetMaterial()->SetReflection( 0 );
		primitive[prim]->GetMaterial()->SetRefraction( 0 );
		primitive[prim]->GetMaterial()->SetSpecular( 0.6f );
		primitive[prim]->GetMaterial()->SetDiffuse( 0.6f );
		primitive[prim]->GetMaterial()->SetColor( Color( 0.3f, 1.0f, 0.4f ) );
		prim++;
	}
	// set number of primitives
	primcnt = prim;

}

};

