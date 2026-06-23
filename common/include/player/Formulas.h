#ifndef FORMULAS_H
#define FORMULAS_H

#include "PlayerClass.h"
#include "Race.h"
#include <cstdint>
#include <tuple>

class Formulas {
public:
  static uint32_t calcularVidaMax(uint32_t constitucion, const Race race,
                                  const PlayerClass playerClass,
                                  uint32_t nivel);
  static uint32_t calcularManaMax(uint32_t inteligencia, const Race race,
                                  const PlayerClass playerClass,
                                  uint32_t nivel);
  static uint32_t calcularRecuperacionVida(const Race race, uint32_t segundos);
  static uint32_t calcularRecuperacionMana(const Race race, uint32_t segundos);
  static uint32_t calcularRecuperacionMeditacion(const PlayerClass playerClass,
                                                 uint32_t inteligencia,
                                                 uint32_t segundos);
  static uint32_t calcularOroMax(uint32_t nivel);
  static uint32_t calcularOroExceso(uint32_t oroActual, uint32_t nivel);
  static uint32_t calcularLimiteXP(uint32_t nivel);
  static uint32_t calcularExperiencia(uint32_t daño, uint32_t nivelAtacante,
                                      uint32_t nivelVictima);
  static uint32_t calcularExperienciaMuerte(uint32_t vidaMax,
                                            uint32_t nivelAtacante,
                                            uint32_t nivelVictima,
                                            double randomFactor);
  static uint32_t calcularDaño(uint32_t fuerza, uint32_t dañoMin,
                               uint32_t dañoMax, double randomFactor);
  static bool calcularEsquivo(uint32_t agilidad, double randomValue);
  static bool calcularCritico(double randomValue);
  static uint32_t calcularDefensa(uint32_t armaduraMin, uint32_t armaduraMax,
                                  uint32_t escudoMin, uint32_t escudoMax,
                                  uint32_t cascoMin, uint32_t cascoMax,
                                  double randomArmadura, double randomEscudo,
                                  double randomCasco);
  static uint32_t calcularOroDropNPC(uint32_t vidaMaxNPC, double randomFactor);
  static uint32_t calcularOroPerdidoMuerte(uint32_t oroActual, uint32_t nivel);
  static std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>
  getRaceStats(const Race race);
  static std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>
  getPlayerClassStats(const PlayerClass playerClass);

private:
  static double getFRazaVida(const Race race);
  static double getFRazaMana(const Race race);
  static double getFRazaRecuperacion(const Race race);
  static double getFClaseVida(const PlayerClass playerClass);
  static double getFClaseMana(const PlayerClass playerClass);
  static double getFClaseMeditacion(const PlayerClass playerClass);
  static bool isGuerrero(const PlayerClass playerClass);
};

#endif
