#include <gtest/gtest.h>

#include "Formulas.h"

TEST(FormulasTest, VidaMax) {
  EXPECT_EQ(Formulas::calcularVidaMax(20, Race::Human, PlayerClass::Warrior, 1),
            28);
}

TEST(FormulasTest, ManaMaxGuerreroSiempreCero) {
  EXPECT_EQ(Formulas::calcularManaMax(100, Race::Elf, PlayerClass::Warrior, 10),
            0);
}

TEST(FormulasTest, ManaMax) {
  EXPECT_EQ(Formulas::calcularManaMax(31, Race::Elf, PlayerClass::Mage, 1), 60);
}

TEST(FormulasTest, Recuperacion) {
  EXPECT_EQ(Formulas::calcularRecuperacionVida(Race::Elf, 10), 12);
  EXPECT_EQ(Formulas::calcularRecuperacionMana(Race::Elf, 10), 12);
}

TEST(FormulasTest, MeditacionGuerreroCero) {
  EXPECT_EQ(
      Formulas::calcularRecuperacionMeditacion(PlayerClass::Warrior, 100, 10),
      0);
}

TEST(FormulasTest, Meditacion) {
  EXPECT_EQ(Formulas::calcularRecuperacionMeditacion(PlayerClass::Mage, 31, 10),
            465);
}

TEST(FormulasTest, OroMax) { EXPECT_EQ(Formulas::calcularOroMax(1), 100); }

TEST(FormulasTest, OroExcesoSinExceso) {
  EXPECT_EQ(Formulas::calcularOroExceso(100, 1), 0);
}

TEST(FormulasTest, OroExcesoConExceso) {
  EXPECT_EQ(Formulas::calcularOroExceso(200, 1), 50);
}

TEST(FormulasTest, LimiteXP) { EXPECT_EQ(Formulas::calcularLimiteXP(1), 1000); }

TEST(FormulasTest, Experiencia) {
  EXPECT_EQ(Formulas::calcularExperiencia(10, 1, 5), 140);
}

TEST(FormulasTest, ExperienciaNivelMenor) {
  EXPECT_EQ(Formulas::calcularExperiencia(10, 10, 1), 10);
}

TEST(FormulasTest, ExperienciaMuerte) {
  EXPECT_EQ(Formulas::calcularExperienciaMuerte(100, 1, 5, 0.5), 70);
}

TEST(FormulasTest, Daño) {
  EXPECT_EQ(Formulas::calcularDaño(13, 2, 5, 0.5), 39);
}

TEST(FormulasTest, Esquivo) {
  EXPECT_TRUE(Formulas::calcularEsquivo(50, 0.01));
  EXPECT_FALSE(Formulas::calcularEsquivo(10, 0.5));
}

TEST(FormulasTest, Critico) {
  EXPECT_TRUE(Formulas::calcularCritico(0.01));
  EXPECT_FALSE(Formulas::calcularCritico(0.5));
}

TEST(FormulasTest, Defensa) {
  EXPECT_EQ(Formulas::calcularDefensa(2, 6, 1, 2, 1, 4, 0.5, 0.5, 0.5), 7);
}

TEST(FormulasTest, OroDropNPC) {
  EXPECT_EQ(Formulas::calcularOroDropNPC(100, 0.5), 10);
}
