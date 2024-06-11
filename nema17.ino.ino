const int DIR = 12;
const int STEP = 14;
const int  steps_per_rev = 200;
const int bouton11 = 25;
const int bouton12 = 26;
const int bouton21 = 32;
const int bouton22 = 33;

int positions[] = {0, 252, 504, 756, 1008}; // Positions cibles autour de 360°
int current_position = 0; // Position actuelle du moteur
bool is_moving = false; // Indique si le moteur est en mouvement
int button1_pressed;
int button2_pressed;
//bool button1_pressed = false; // Indique si le bouton 1 est pressé
//bool button2_pressed = false; // Indique si le bouton 2 est pressé

// Variables pour contrôle de vitesse
unsigned long previousMicros = 0;
unsigned long interval = 1000; // Délai initial entre chaque pas en microsecondes
unsigned long serialDelay = 1000; // Délai entre chaque message sériel

void setup()
{
  Serial.begin(115200);
  pinMode(bouton11, OUTPUT); // Utilisation de la résistance de pull-up interne
  pinMode(bouton12, INPUT); // Utilisation de la résistance de pull-up interne
  pinMode(bouton21, OUTPUT); // Utilisation de la résistance de pull-up interne
  pinMode(bouton22, INPUT); // Utilisation de la résistance de pull-up interne
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  digitalWrite(bouton11, HIGH);
  digitalWrite(bouton21, HIGH);

  Serial.println("Setup terminé");
  Serial.println("Moteur à l'arrêt.");
}

void loop()
{
  // Lecture de l'état des boutons
  button1_pressed = digitalRead(bouton12);
  button2_pressed = digitalRead(bouton22);

  // Affichage de l'état des boutons
  Serial.print("Bouton 1 : ");
  Serial.println(button1_pressed);
  Serial.print("Bouton 2 : ");
  Serial.println(button2_pressed);

  // Vérification si le moteur est déjà en mouvement
  if (is_moving) {
    Serial.println("Le moteur est déjà en mouvement.");
  }

  // Gestion du mouvement en fonction de l'état des boutons
  if (button1_pressed && !is_moving) {
    Serial.println("Déclenchement du mouvement dans le sens antihoraire...");
    moveMotor(false); // Sens antihoraire
  } else if (button2_pressed && !is_moving) {
    Serial.println("Déclenchement du mouvement dans le sens horaire...");
    moveMotor(true); // Sens horaire
  }

  // Ajout d'un petit délai pour éviter la répétition rapide des boucles
  delay(50);
}

// Fonction pour déplacer le moteur vers la prochaine position
void moveMotor(bool clockwise) {
  Serial.println("Début du mouvement...");

  // Indication que le moteur est en mouvement
  is_moving = true;

  // Détermination de la direction du mouvement
  digitalWrite(DIR, clockwise ? LOW : HIGH);

  // Calcul de la prochaine position
  int next_position = current_position;
  if (clockwise) {
    next_position++;
    if (next_position >= sizeof(positions) / sizeof(positions[0])) {
      next_position = 0;
    }
  } else {
    next_position--;
    if (next_position < 0) {
      next_position = sizeof(positions) / sizeof(positions[0]) - 1;
    }
  }

  // Affichage de la position actuelle et de la position cible
  Serial.print("Position actuelle : ");
  Serial.println(current_position);
  Serial.print("Prochaine position : ");
  Serial.println(positions[next_position]);

  // Mouvement du moteur
  int steps_to_move = abs(positions[next_position] - positions[current_position]) * steps_per_rev / 360;
  for (int i = 0; i < steps_to_move; i++) {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(interval);
    digitalWrite(STEP, LOW);
    delayMicroseconds(interval);
  }

  // Mise à jour de la position actuelle
  current_position = next_position;

  // Indication que le mouvement est terminé
  is_moving = false;

  Serial.println("Mouvement terminé");

  // Délai pour réduire la vitesse des messages sériels
  delay(serialDelay);
}
