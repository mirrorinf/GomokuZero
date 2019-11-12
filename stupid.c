#include <stdio.h>

int main() {
	int u;
	u = sizeof(void *)*227 + sizeof(int) * 6 + sizeof(float) + sizeof(char)*225;
	printf("%d\n", u);
	return 0;
}
