#pragma once
#include <Arduino.h>
#include <String.h>

#ifndef UTILS

void printString(bool debug, String info) {
  if (debug) {
    Serial.print(info);
  }
}

void printStringLn(bool debug, String info) {
  if (debug) {
    Serial.println(info);
  }
}

void printChar(bool debug, char* info) {
  if (debug) {
    Serial.print(info);
  }
}

void printCharLn(bool debug, char* info) {
  if (debug) {
    Serial.println(info);
  }
}

void printDouble(bool debug, double info) {
  if (debug) {
    Serial.print(info);
  }
}

void printDoubleLn(bool debug, double info) {
  if (debug) {
    Serial.println(info);
  }
}

void pintInt(bool debug, int info) {
  if (debug) {
    Serial.println(info);
  }
}

void printIntLn(bool debug, int info) {
  if (debug) {
    Serial.println(info);
  }
}

#endif
