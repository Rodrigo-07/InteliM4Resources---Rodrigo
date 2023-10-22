void setup()
{
  pinMode(3, OUTPUT); // Configurar a porta 3 para ser a saída de energia
}

void loop()
{
  digitalWrite(3, HIGH); // Definir a voltagem máxima (5V) na porta 3
  
  delay(1000); // Delay de 1 segundo
  
  digitalWrite(3, LOW); // Definir a voltagem minima (0V) na porta 3
  
  delay(1000); // Delay de 1 segundo
}