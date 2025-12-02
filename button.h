#ifndef BUTTON_H
#define BUTTON_H

namespace button {
  int reportedState[btnCount] = {LOW};
  int lastState[btnCount] = {LOW};
  unsigned long lastDebounceTime[btnCount] = {0};
  unsigned long debounceDelay = 50;

  enum Button {
    NEXT,
    SELECT,
    MENU
  };

  // Should return true once when determined that button has been pressed
  bool pressed(int btnID) {
    bool result = false;
    int newState = digitalRead(btnPin[btnID]);

    if (newState != lastState[btnID]) {
      // If the state has fluctuated, set a cooldown timer.
      lastDebounceTime[btnID] = millis();
    }

    // Only progress button logic if cooldown timer elapses => any fluctations have settled.
    if ((millis() - lastDebounceTime[btnID]) > debounceDelay) {
      // Reported state is the current state the program considers the button to be reporting.
      if (newState != reportedState[btnID]) {
        reportedState[btnID] = newState;

        if (reportedState[btnID] == HIGH) {
          result = true;
        }
      }
    }
    lastState[btnID] = newState;
    return result;
  }

}


#endif // BUTTON_H