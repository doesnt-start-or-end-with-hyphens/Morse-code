//The database containing all The morse Translations
struct database {
  char letter;
  const bool* morse;
  uint8_t len;
};

#define INPUT_CHAR(character, ...) { character, (const bool[]){__VA_ARGS__}, sizeof((const bool[]){__VA_ARGS__}) }

database morseIndex[] = {
  INPUT_CHAR('a', 0, 1),
  INPUT_CHAR('b', 1, 0, 0, 0),
  INPUT_CHAR('c', 1, 0, 1, 0),
  INPUT_CHAR('d', 1, 0, 0),
  INPUT_CHAR('e', 0),
  INPUT_CHAR('f', 0, 0, 1, 0),
  INPUT_CHAR('g', 1, 1, 0),
  INPUT_CHAR('h', 0, 0, 0, 0),
  INPUT_CHAR('i', 0, 0),
  INPUT_CHAR('j', 0, 1, 1, 1),
  INPUT_CHAR('k', 1, 0, 1),
  INPUT_CHAR('l', 0, 1, 0, 0),
  INPUT_CHAR('m', 1, 1),
  INPUT_CHAR('n', 1, 0),
  INPUT_CHAR('o', 1, 1, 1),
  INPUT_CHAR('p', 0, 1, 1, 0),
  INPUT_CHAR('q', 1, 1, 0, 1),
  INPUT_CHAR('r', 0, 1, 0),
  INPUT_CHAR('s', 0, 0, 0),
  INPUT_CHAR('t', 1),
  INPUT_CHAR('u', 0, 0, 1),
  INPUT_CHAR('v', 0, 0, 0, 1),
  INPUT_CHAR('w', 0, 1, 1),
  INPUT_CHAR('x', 1, 0, 0, 1),
  INPUT_CHAR('y', 1, 0, 1, 1),
  INPUT_CHAR('z', 1, 1, 0, 0),
  INPUT_CHAR('1', 0, 1, 1, 1, 1),
  INPUT_CHAR('2', 0, 0, 1, 1, 1),
  INPUT_CHAR('3', 0, 0, 0, 1, 1),
  INPUT_CHAR('4', 0, 0, 0, 0, 1),
  INPUT_CHAR('5', 0, 0, 0, 0, 0),
  INPUT_CHAR('6', 1, 0, 0, 0, 0),
  INPUT_CHAR('7', 1, 1, 0, 0, 0),
  INPUT_CHAR('8', 1, 1, 1, 0, 0),
  INPUT_CHAR('9', 1, 1, 1, 1, 0),
  INPUT_CHAR('0', 1, 1, 1, 1, 1),
  INPUT_CHAR('?', 0, 0, 1, 1, 0, 0),
  INPUT_CHAR('!', 1, 0, 1, 0, 1, 1),
  INPUT_CHAR('.', 0, 1, 0, 1, 0, 1),
  INPUT_CHAR(',', 1, 1, 0, 0, 1, 1),
  INPUT_CHAR(';', 1, 0, 1, 0, 1, 0),
  INPUT_CHAR(':', 1, 1, 1, 0, 0, 0),
  INPUT_CHAR('+', 0, 1, 0, 1, 0),
  INPUT_CHAR('-', 1, 0, 0, 0, 0, 1),
  INPUT_CHAR('/', 1, 0, 0, 1, 0),
  INPUT_CHAR('=', 1, 0, 0, 0, 1)
};
String quizWords[] = {
  "Hello",
  "Mate",
  "greeting",
  "testing",
  "lol",
  "bye",
  "copy",
  "finished",
  "dinner",
  "table",
  "cop",
  "fish",
  "cow",
  "pig",
  "sheep",
  "chicken",
  "house",
  "control",
  "morse",
  "arduino"
};