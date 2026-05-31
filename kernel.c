int cursor = 0;
char color = 0x07;

void putInMemory(int segment, int address, char character);
int getChar();

void printChar(char c);
void printString(char* str);
void newline();
void clearScreen();
void readString(char* str);

void printChar(char c) {
    putInMemory(0xB800, cursor * 2, c);
    putInMemory(0xB800, cursor * 2 + 1, color);
    cursor++;
}

void newline() {
    cursor = ((cursor / 80) + 1) * 80;
}
void printString(char* str) {
    int i = 0;

    while (str[i] != 0) {
        printChar(str[i]);
        i++;
    }
}
void clearScreen() {
    int i;

    cursor = 0;

    for (i = 0; i < 80 * 25; i++) {
        putInMemory(0xB800, i * 2, ' ');
        putInMemory(0xB800, i * 2 + 1, color);
    }
}
void readString(char* str) {
    int i = 0;
    char c;
while (1) {
    c = getChar();

    if (c == 13) {
        str[i] = 0;
        break;
    }
    else if (c == 8) {
        if (i > 0) {
            i--;
            cursor--;
            printChar(' ');
            cursor--;
        }
    }
    else {
        str[i] = c;
        printChar(c);
        i++;
    }
}
}
int strcmp(char* a, char* b) {
    int i = 0;

    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) {
            return 0;
        }
        i++;
    }

    return a[i] == b[i];
}
int startsWith(char* str, char* prefix) {
    int i = 0;

    while (prefix[i] != 0) {
        if (str[i] != prefix[i]) {
            return 0;
        }
        i++;
    }

    return 1;
}
int atoi(char* str) {
    int result = 0;
    int i = 0;
    int neg = 0;

    if (str[0] == '-') {
        neg = 1;
        i = 1;
    }

    while (str[i] != 0) {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    if (neg) return -result;
    return result;
}
void intToString(int num, char* str) {

    if (num < 0) {
        str[0] = '-';
        num = -num;

        if (num < 10) {
            str[1] = num + '0';
            str[2] = 0;
        }
        else if (num < 100) {
            str[1] = (num / 10) + '0';
            str[2] = (num - ((num / 10) * 10)) + '0';
            str[3] = 0;
        }
        else {
            str[1] = (num / 100) + '0';

            num = num - ((num / 100) * 100);

            str[2] = (num / 10) + '0';
            str[3] = (num - ((num / 10) * 10)) + '0';
            str[4] = 0;
        }

        return;
    }

    if (num < 10) {
        str[0] = num + '0';
        str[1] = 0;
    }
    else if (num < 100) {
        str[0] = (num / 10) + '0';
        str[1] = (num - ((num / 10) * 10)) + '0';
        str[2] = 0;
    }
    else {
        str[0] = (num / 100) + '0';

        num = num - ((num / 100) * 100);

        str[1] = (num / 10) + '0';
        str[2] = (num - ((num / 10) * 10)) + '0';
        str[3] = 0;
    }
}
int factorial(int n) {
    int hasil = 1;
    int i;

    for (i = 1; i <= n; i++) {
        hasil = hasil * i;
    }

    return hasil;
}
void main() {

    char cmd[64];

    clearScreen();

    printString("Hello");
    newline();

    printString("type 'help'");
    newline();
    newline();

    while (1) {

        printString("> ");

        readString(cmd);

        newline();
if (strcmp(cmd, "check")) {
    printString("ok");
}
else if (startsWith(cmd, "add ")) {

    char num1[10];
    char num2[10];
    char hasil[10];

    int i = 4;
    int j = 0;
    int a;
    int b;

    while (cmd[i] != ' ' && cmd[i] != 0) {
        num1[j] = cmd[i];
        i++;
        j++;
    }
    num1[j] = 0;

    i++;
    j = 0;

    while (cmd[i] != 0) {
        num2[j] = cmd[i];
        i++;
        j++;
    }
    num2[j] = 0;

    a = atoi(num1);
    b = atoi(num2);

    intToString(a + b, hasil);

    printString(hasil);
}
else if (startsWith(cmd, "sub ")) {

    char num1[10];
    char num2[10];
    char hasil[10];

    int i = 4;
    int j = 0;
    int a;
    int b;

    while (cmd[i] != ' ' && cmd[i] != 0) {
        num1[j] = cmd[i];
        i++;
        j++;
    }
    num1[j] = 0;

    i++;
    j = 0;

    while (cmd[i] != 0) {
        num2[j] = cmd[i];
        i++;
        j++;
    }
    num2[j] = 0;

    a = atoi(num1);
    b = atoi(num2);

    intToString(a - b, hasil);

    printString(hasil);
}
else if (startsWith(cmd, "fac ")) {

    char angka[10];
    char hasil[10];

    int i = 4;
    int j = 0;
    int n;

    while (cmd[i] != 0) {
        angka[j] = cmd[i];
        i++;
        j++;
    }

    angka[j] = 0;

    n = atoi(angka);

    intToString(factorial(n), hasil);

    printString(hasil);
}
else if (startsWith(cmd, "season ")) {

    if (strcmp(cmd, "season winter")) {
        printString("12 1 2");
    }
    else if (strcmp(cmd, "season spring")) {
        printString("3 4 5");
    }
    else if (strcmp(cmd, "season summer")) {
        printString("6 7 8");
    }
    else if (strcmp(cmd, "season fall")) {
        printString("9 10 11");
    }
    else if (strcmp(cmd, "season radiant")) {
        printString("all months");
    }
    else {
        printString("season not found");
    }
}
else if (startsWith(cmd, "triangle ")) {

    char angka[10];
    int n;
    int i;
    int j;

    i = 9;
    j = 0;

    while (cmd[i] != 0) {
        angka[j] = cmd[i];
        i++;
        j++;
    }

    angka[j] = 0;

    n = atoi(angka);

    for (i = 1; i <= n; i++) {

        for (j = 0; j < i; j++) {
            printChar('*');
        }

        newline();
    }
}
else if (strcmp(cmd, "clear")) {
    clearScreen();
}
else if (strcmp(cmd, "about")) {
    printString("Donna Shell v1.0");
}
else if (strcmp(cmd, "help")) {
    printString("check add sub fac season triangle clear about help");
}
else {
    printString("unknown command");
}
        newline();
    }
}
