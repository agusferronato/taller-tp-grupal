#include "Formulas.h"

#include <algorithm>
#include <cctype>
#include <cmath>

static const double EXP_ORO_XP = 1.5;

static double clamp01(double value) {
  return std::max(0.0, std::min(1.0, value));
}

bool Formulas::isGuerrero(const PlayerClass playerClass) {
  return playerClass == PlayerClass::Warrior;
}

double Formulas::getFRazaVida(const Race race) {
  switch (race) {
  case Race::Human:
    return 1;
  case Race::Elf:
    return 0.8;
  case Race::Dwarf:
    return 1.3;
  case Race::Gnome:
    return 1.1;
  }
  return 0;
}

double Formulas::getFRazaMana(const Race race) {
  switch (race) {
  case Race::Human:
    return 1;
  case Race::Elf:
    return 1.3;
  case Race::Dwarf:
    return 0.7;
  case Race::Gnome:
    return 1.2;
  default:
    return 0;
  }
}

double Formulas::getFRazaRecuperacion(const Race race) {
  switch (race) {
  case Race::Human:
    return 1;
  case Race::Elf:
    return 1.2;
  case Race::Dwarf:
    return 0.8;
  case Race::Gnome:
    return 0.9;
  default:
    return 0;
  }
}

double Formulas::getFClaseVida(const PlayerClass playerClass) {
  switch (playerClass) {
  case PlayerClass::Warrior:
    return 1.4;
  case PlayerClass::Mage:
    return 0.7;
  case PlayerClass::Priest:
    return 1.0;
  case PlayerClass::Paladin:
    return 1.2;
  default:
    return 0;
  }
}

double Formulas::getFClaseMana(const PlayerClass playerClass) {
  switch (playerClass) {
  case PlayerClass::Warrior:
    return 0.0;
  case PlayerClass::Mage:
    return 1.5;
  case PlayerClass::Priest:
    return 1.2;
  case PlayerClass::Paladin:
    return 0.8;
  default:
    return 1.0;
  }
}

double Formulas::getFClaseMeditacion(const PlayerClass playerClass) {
  switch (playerClass) {
  case PlayerClass::Mage:
    return 1.5;
  case PlayerClass::Priest:
    return 1.2;
  case PlayerClass::Paladin:
    return 0.5;
  case PlayerClass::Warrior:
    return 0.0;
  default:
    return 1;
  }
}

uint32_t Formulas::calcularVidaMax(uint32_t constitucion, const Race race,
                                   const PlayerClass playerClass,
                                   uint32_t nivel) {
  double resultado = static_cast<double>(constitucion) *
                     getFClaseVida(playerClass) * getFRazaVida(race) *
                     static_cast<double>(nivel);
  return static_cast<uint32_t>(std::round(resultado));
}

uint32_t Formulas::calcularManaMax(uint32_t inteligencia, const Race race,
                                   const PlayerClass playerClass,
                                   uint32_t nivel) {
  if (isGuerrero(playerClass))
    return 0;
  double resultado = static_cast<double>(inteligencia) *
                     getFClaseMana(playerClass) * getFRazaMana(race) *
                     static_cast<double>(nivel);
  return static_cast<uint32_t>(std::round(resultado));
}

uint32_t Formulas::calcularRecuperacionVida(const Race race,
                                            uint32_t segundos) {
  double resultado = getFRazaRecuperacion(race) * static_cast<double>(segundos);
  return static_cast<uint32_t>(std::round(resultado));
}

uint32_t Formulas::calcularRecuperacionMana(const Race race,
                                            uint32_t segundos) {
  double resultado = getFRazaRecuperacion(race) * static_cast<double>(segundos);
  return static_cast<uint32_t>(std::round(resultado));
}

uint32_t Formulas::calcularRecuperacionMeditacion(const PlayerClass playerClass,
                                                  uint32_t inteligencia,
                                                  uint32_t segundos) {
  if (isGuerrero(playerClass))
    return 0;
  double resultado = getFClaseMeditacion(playerClass) *
                     static_cast<double>(inteligencia) *
                     static_cast<double>(segundos);
  return static_cast<uint32_t>(std::round(resultado));
}

uint32_t Formulas::calcularOroMax(uint32_t nivel) {
  double resultado = 100.0 * std::pow(static_cast<double>(nivel), EXP_ORO_XP);
  return static_cast<uint32_t>(std::round(resultado));
}

uint32_t Formulas::calcularOroExceso(uint32_t oroActual, uint32_t nivel) {
  uint32_t oroMax = calcularOroMax(nivel);
  uint32_t tope = oroMax + oroMax / 2;
  if (oroActual <= oroMax)
    return 0;
  if (oroActual > tope)
    return tope - oroMax;
  return oroActual - oroMax;
}

uint32_t Formulas::calcularLimiteXP(uint32_t nivel) {
  double resultado = 1000.0 * std::pow(static_cast<double>(nivel), EXP_ORO_XP);
  return static_cast<uint32_t>(std::round(resultado));
}

uint32_t Formulas::calcularExperiencia(uint32_t daño, uint32_t nivelAtacante,
                                       uint32_t nivelVictima) {
  int diff =
      static_cast<int>(nivelVictima) - static_cast<int>(nivelAtacante) + 10;
  if (diff < 0)
    diff = 0;
  return daño * static_cast<uint32_t>(diff);
}

uint32_t Formulas::calcularExperienciaMuerte(uint32_t vidaMax,
                                             uint32_t nivelAtacante,
                                             uint32_t nivelVictima,
                                             double randomFactor) {
  double factor = clamp01(randomFactor) * 0.1;
  int diff =
      static_cast<int>(nivelVictima) - static_cast<int>(nivelAtacante) + 10;
  if (diff < 0)
    diff = 0;
  double resultado =
      factor * static_cast<double>(vidaMax) * static_cast<double>(diff);
  return static_cast<uint32_t>(std::round(resultado));
}

uint32_t Formulas::calcularDaño(uint32_t fuerza, uint32_t dañoMin,
                                uint32_t dañoMax, double randomFactor) {
  double r = clamp01(randomFactor);
  uint32_t daño = dañoMin + static_cast<uint32_t>(
                                r * static_cast<double>(dañoMax - dañoMin));
  return fuerza * daño;
}

bool Formulas::calcularEsquivo(uint32_t agilidad, double randomValue) {
  double probabilidad = static_cast<double>(agilidad) * 0.01;
  return clamp01(randomValue) < probabilidad;
}

bool Formulas::calcularCritico(double randomValue) {
  return clamp01(randomValue) < 0.05;
}

uint32_t Formulas::calcularDefensa(uint32_t armaduraMin, uint32_t armaduraMax,
                                   uint32_t escudoMin, uint32_t escudoMax,
                                   uint32_t cascoMin, uint32_t cascoMax,
                                   double randomArmadura, double randomEscudo,
                                   double randomCasco) {
  auto valor = [](uint32_t min, uint32_t max, double r) -> uint32_t {
    double roll = clamp01(r);
    return min + static_cast<uint32_t>(roll * static_cast<double>(max - min));
  };
  return valor(armaduraMin, armaduraMax, randomArmadura) +
         valor(escudoMin, escudoMax, randomEscudo) +
         valor(cascoMin, cascoMax, randomCasco);
}

uint32_t Formulas::calcularOroDropNPC(uint32_t vidaMaxNPC,
                                      double randomFactor) {
  double r = clamp01(randomFactor) * 0.2;
  return static_cast<uint32_t>(r * static_cast<double>(vidaMaxNPC));
}

uint32_t Formulas::calcularOroPerdidoMuerte(uint32_t oroActual,
                                            uint32_t nivel) {
  return calcularOroExceso(oroActual, nivel);
}

std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>
Formulas::getRaceStats(const Race race) {
  switch (race) {
  case Race::Human:
    return {10, 10, 10, 10};
  case Race::Elf:
    return {6, 13, 5, 16};
  case Race::Dwarf:
    return {13, 4, 16, 7};
  case Race::Gnome:
    return {7, 6, 14, 13};
  default:
    return {0, 0, 0, 0};
  }
}

std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>
Formulas::getPlayerClassStats(const PlayerClass playerClass) {
  switch (playerClass) {
  case PlayerClass::Mage:
    return {3, 5, 5, 15};
  case PlayerClass::Priest:
    return {7, 7, 9, 10};
  case PlayerClass::Paladin:
    return {10, 6, 10, 8};
  case PlayerClass::Warrior:
    return {10, 8, 10, 3};
  default:
    return {0, 0, 0, 0};
  }
}
