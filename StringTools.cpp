
void GetStringFromNumber(char *Output, int number, int Digits)
{//1234
 // Based on a generic itoa code without the null termination
	int base = 10;
	unsigned char *str = (unsigned char*)Output;
	unsigned char buffer = 0;
	int i = 0;
	int j = 0;
	bool isNegative = false;
	while (i < Digits)
	{
		Output[i] = '0';
		i++;
	}
	Output[i] = '\0';
	i = 0;
	/* Handle 0 explicitely, otherwise empty string is printed for 0 */
	if (number == 0)
	{
		//		str[i++] = '0';
		return;
	}

	/* == CODE NOT NEEDED
	// In standard itoa(), negative numbers are handled only with
	// base 10. Otherwise numbers are considered unsigned.
	if (number < 0 && base == 10)
	{
	isNegative = true;
	number = -number;
	}
	*/
	// Process individual digits
	while (number != 0)
	{
		int rem = number % base;
		str[Digits - 1 - i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		number = number / base;
	}
	/* == CODE NOT NEEDED
	// If number is negative, append '-'
	if (isNegative)
	str[i++] = '-';
	*/

	/* == CODE NOT NEEDED
	// Reverse the string
	i--;
	while (j<i) {
	buffer = str[j];// pega inicio
	str[j] = str[i];// inicio recebe final
	str[i] = buffer;// final recebe inicio
	j++;
	i--;
	}
	*/

}
