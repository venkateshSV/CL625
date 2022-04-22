/*qbits are in superposition and can be entangled (correlated)
 * We substitute superposition by biased randomness and entanglement by a depending equation
*/
int aPercentOne, bPercentOne;
int aQubit, bQubit; //qubits
bool aCollapse, bCollapse;
String collapsed;
String entangled;
int correlated; //0: not correlated, 1: correlated, 2 error;
bool error;
#define doNothing 2
#define LEDq2 6 //they represent the state of qubits
#define LEDq1 7

void setup() {
  randomSeed(analogRead(A0));
  Serial.begin(115200);
  
  //ask parametres by Serial Monitor
  Serial.print("\nProbabillity of first qubit of being a 1.  0%-100%:");
  while (!Serial.available()) {};
  aPercentOne = Serial.readString().toInt();
  printQubit(aPercentOne);
  
  Serial.print("Probablillity of second qubit of being a 1.  0%-100%:");
  while (!Serial.available()) {};
  bPercentOne = Serial.readString().toInt();
  printQubit(bPercentOne);
  //qubits entangled
  do {
    Serial.print("Are those qubits entangled? Y/N:");
    while (!Serial.available()) {};
    entangled = Serial.readString();
    entangled.trim();
    correlated = (entangled == "Y" || entangled == "y") ? 1 : ((entangled == "N" || entangled == "n") ? 0 : doNothing) ;
  } while (correlated == doNothing);
  Serial.print(" ");
  correlated ? Serial.println("Qubits ENTANGLED") : Serial.println("not entangled");
  if (correlated) {
    Serial.print("Both qubits are entangled. Which qubit collapses firts? A/B:");
    while (!Serial.available()) {};
    collapsed = Serial.readString();
    collapsed.trim(); //do not forget
    collapsed.toLowerCase();
    Serial.print(" ");
    Serial.println(collapsed);
    aCollapse = collapsed == "a" ? 1 : 0;
    bCollapse = collapsed == "b" ? 1 : 0;
  }
}

void loop() {
  //the following are the most interested formulas of the present firmware
  //collapse qubits and show with LEDs
  //entangled, use the same formula
  correlated ? (bQubit = aQubit = (aCollapse ? collapse(aPercentOne) : collapse(bPercentOne))) : (correlated = 0); //qubits entangled
  //not entangled, use different formulas
  !correlated ? aQubit = collapse(aPercentOne) : doNothing; //if not entangled collapse
  !correlated ? bQubit = collapse(bPercentOne) : doNothing; //if not entangled collapse
  //show by Serial Monitor
  Serial.print("ba = ");
  Serial.print(bQubit); digitalWrite(LEDq2, bQubit); //present the qubits by Serial Monitor and visually
  //show by LEDs
  Serial.println(aQubit); digitalWrite(LEDq1, aQubit);
  delay(1000);
}
//Ex. percent of be a one: 25%
bool collapse(int percentOne) {
  int percentZero = 100 - percentOne;
  return random(100 - percentZero, 100 + percentOne) > 100 ? 1 : 0;
}

void printQubit(int percentOne){
  
  Serial.println();
  //when qubits are |0> or |1>
  if (percentOne == 0 || percentOne == 100) {
    percentOne == 100 ? Serial.println("|1>") : doNothing;
    percentOne == 0 ? Serial.println("|0>") : doNothing;
  }
  else {
  Serial.print("((√");
  Serial.print(100-percentOne); //print percentages of first qubit
  Serial.print(")/10)|0> + ");
  Serial.print("((√");
  Serial.print(percentOne);
  Serial.println(")/10)|1>\n");
  }
}
