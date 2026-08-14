/**
 * AEL Sovereign Systems — Custom CS50 Masterpiece (ael_src1)
 * Project: Orbital Physics Engine & Escape Velocity Simulator (ael_stellar_orbit.c)
 * Description: Demonstrates floating-point casting, scientific notation in C, math library
 *              integrations (<math.h>), and handling large memory limits without overflow.
 */

#include <cs50.h>
#include <math.h>
#include <stdio.h>

// Universal Gravitational Constant (G) in m^3 kg^-1 s^-2
#define GRAV_CONST 6.67430e-11

double calculate_escape_velocity(double mass, double radius);

int main(void)
{
    printf("====================================================\n");
    printf("     AEL SUB-SILICON STELLAR ORBIT DYNAMICS v3.0\n");
    printf("====================================================\n");

    // Earth baseline parameters
    double earth_mass = 5.9722e24; // in kg
    double earth_radius = 6371000; // in meters (6,371 km)

    printf("Planetary Body: Earth (Terra)\n");
    printf("Mass: %e kg | Radius: %.2f km\n", earth_mass, earth_radius / 1000.0);

    double esc_vel = calculate_escape_velocity(earth_mass, earth_radius);
    
    printf("\n⚡ Mathematical Result:\n");
    printf("--> Escape Velocity: %.2f meters/sec (approx %.2f km/sec)\n", esc_vel, esc_vel / 1000.0);
    printf("====================================================\n");

    // Interactive Custom Input
    printf("\n[Custom Simulation Entry]\n");
    double custom_mass = get_double("Enter Custom Mass (kg): ");
    double custom_radius = get_double("Enter Custom Radius (m): ");

    if (custom_mass <= 0 || custom_radius <= 0)
    {
        printf("⚠️ Error: Mass and radius must be positive non-zero values.\n");
        return 1;
    }

    double custom_esc = calculate_escape_velocity(custom_mass, custom_radius);
    printf("--> Custom Planetary Escape Velocity: %.2f km/sec\n", custom_esc / 1000.0);
    printf("====================================================\n");

    return 0;
}

/**
 * Calculates escape velocity using formula: v = sqrt( (2 * G * M) / R )
 */
double calculate_escape_velocity(double mass, double radius)
{
    return sqrt((2.0 * GRAV_CONST * mass) / radius);
}
