from math import exp

# Returns water conten in g/m^3 given temperature in celcius
def absolute_humidity(temperature: float, relative_humidity:float) -> float:
    t, rh = temperature, relative_humidity

    # Partial pressure of water vapor at 100% rh in hectopascals at temperature (celcius)
    saturation_pressure = 6.112 * exp( (17.67 * t) / (t + 243.5) )

    return saturation_pressure * rh * 2.1674 / (273.15 + t)

if __name__ == "__main__":
    print(f"t=10c rh=100% mass={absolute_humidity(10, 100):.3f}g/m^3")
    print(f"t=20c rh=100% mass={absolute_humidity(20, 100):.3f}g/m^3")
    print(f"t=30c rh=100% mass={absolute_humidity(30, 100):.3f}g/m^3")