#include <LiquidCrystalRus.h>

class NimGame {
  private:
    bool player_;
    bool game_started_;
    bool game_active_;
    bool game_mode_;

    uint8_t pile_;
    uint8_t turn_;

    uint8_t limit_ = 3;

  public:
    NimGame() {
      game_started_ = false;
      game_mode_ = 0;
      game_active_ = true;
      pile_ = 0;
      player_ = 0;
      turn_ = 0;
    }

    void ModifyPile(uint8_t num) {
      pile_ += num;
    }

    int GetPile() {
      return pile_;
    }

    bool GetPlayer() {
      return player_;
    }

    bool GetGameStart() {
      return game_started_;
    }

    bool GetGameStatus() {
      return game_active_;
    }

    bool GetGameMode() {
      return game_mode_;
    }

    void Init();

    void Turn();

    void End();

    void Restart();

    void AITurn();
};


class LED {
  private:
    uint8_t pin_;

  public:
    LED(uint8_t pin) : pin_(pin) {}

    void init() {
      pinMode(pin_, OUTPUT);
    }

    void Light(bool state) {
      if (state) {
        digitalWrite(pin_, HIGH);
      }
      else {
        digitalWrite(pin_, LOW);
      }
    }
};

class Button {
  private:
    bool state_;
    uint8_t pin_;

  public:
    Button(uint8_t pin) : pin_(pin) {}

    void init() {
      pinMode(pin_, INPUT_PULLUP);
      state_ = digitalRead(pin_);
    }

    bool IsPressed() {
      bool value = digitalRead(pin_);
      if (value != state_) {
        state_ = value;
        if (state_) {
          return true;
        }
      }
      return false;
    }
};
// RS, E, D4, D5, D6, D7
LiquidCrystalRus display(13, 12, 11, 10, 9, 8);

NimGame Game;

Button  FirstButton(5);
Button  SecondButton(4);
Button  ModeButton(2);
LED     PlayerLed(7);
LED     ModeLed(6);

void setup() {
  PlayerLed.init();
  ModeLed.init();
  FirstButton.init();
  SecondButton.init();
  ModeButton.init();
  display.begin(16, 2);
  display.print("   Игра  Баше   ");
}

void loop() {

  if (!Game.GetGameStart() && !Game.GetGameStatus()) {
    Game.Restart();
  }
  else if (!Game.GetGameStart()) {
    Game.Init();
  }
  else {
    if (Game.GetPile() == 0) {
      Game.End();
    }
    else if (Game.GetGameMode()){
      if (Game.GetPlayer() == 1){
        Game.Turn();
      }
      else {
        Game.AITurn();
      }
    }
    else {
      Game.Turn();
    }
  }
}

void NimGame::Init() {
  display.setCursor(0, 0);
  display.print("   Игра  Баше   ");
  if (pile_ > 0) {

    display.setCursor(0, 1);
    display.print("Размер кучи:");
    display.print(pile_, 10);
  }

  if (FirstButton.IsPressed()) {
    ++pile_;
  }
  if (SecondButton.IsPressed()) {
    if (pile_ != 0) {
      display.setCursor(0, 0);
      display.print(" Игра началась! ");
      game_started_ = true;
      delay(1000);
    }
    else {
      display.setCursor(0, 0);
      display.print(" Добавьте камни ");
    }
  }
  if (ModeButton.IsPressed()) {
    game_mode_ = !game_mode_;
    ModeLed.Light(game_mode_);
  }
}

void NimGame::Turn() {
  if (turn_ < limit_) {
    display.setCursor(0, 0);
    display.print(" Ход игрока ");
    int id = (int)player_ + 1;
    display.print(id, 10);
    display.print("   ");
  }
  display.setCursor(0, 1);
  display.print("Размер кучи:");
  display.print(pile_, 10);
  PlayerLed.Light(player_);
  if (FirstButton.IsPressed()) {
    if (turn_ < limit_) {
      ++turn_;
      --pile_;
    }
    else {
      display.setCursor(0, 0);
      display.print("Ходы закончились");
    }
  }
  if (SecondButton.IsPressed()) {
    player_ = !player_;
    display.setCursor(0, 0);
    display.print(" Ход игрока ");
    int id = (int)player_ + 1;
    display.print(id);
    display.print("   ");
    turn_ = 0;
  }
}

void NimGame::End() {
  display.setCursor(0, 0);
  display.print("Игрок ");
  int id = (int)player_ + 1;
  display.print(id, 10);
  display.print(" победил!");

  PlayerLed.Light(false);

  turn_ = 0;
  game_active_ = false;
  game_started_ = false;
  pile_ = 0;
}

void NimGame::Restart() {
  display.setCursor(0, 1);
  display.print("(1) для рестарта");
  if (FirstButton.IsPressed()) {
    display.clear();
    game_active_ = true;
    player_ = 0;
  }
}

void NimGame::AITurn() {  
  display.setCursor(0, 0);
  display.print("    Ход ИИ    ");
  display.print(pile_, 10);
  display.setCursor(0, 1);
  display.print("Размер кучи:");
  display.print(pile_, 10);
  delay(1000);
  if (turn_ < limit_) {
    if ((pile_ - 1) % (limit_ + 1)) {
      player_ = !player_;
      display.setCursor(0, 0);
      display.print(" Ход игрока ");
      int id = (int)player_ + 1;
      display.print(id);
      turn_ = 0;
    }
   ++turn_;
    --pile_;
  }
}

