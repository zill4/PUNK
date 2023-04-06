#pragma once
#include "DrawDebugHelpers.h"

#define THIRTY 30
#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, FColor::Red, true);
#define DRAW_SPHERE_COLOR(Location, Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 8.f, 24, Color, true, 5.f);
#define DRAW_SPHERE_SingleFrame(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, FColor::Blue, false, -1.f);

#define DRAW_LINE(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 2.f);
#define DRAW_LINE_SingleFrame(StartLocation, EndLocation) if(GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, -1.f, 0, 2.f);

#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true);
#define DRAW_POINT_SingleFrame(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Blue, false, -1.f);

#define DRAW_VECTOR(StarLocation, EndLocation) if (GetWorld()) \
{ \
		DrawDebugLine(GetWorld(), StarLocation, EndLocation, FColor::Red, true, -1.f, 0, 2.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, true); \
}

#define DRAW_VECTOR_SingleFrame(StarLocation, EndLocation) if (GetWorld()) \
{ \
		DrawDebugLine(GetWorld(), StarLocation, EndLocation, FColor::Blue, false, -1.f, 0, 2.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Blue, false, -1.f); \
}