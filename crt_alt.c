

//when calling alloca msvc will embedd a call to the crt function 'chkstk' to make sure that the alloca doesn't overwrite program memory. we need to define it so that we don't get linker errors
void __chkstk(void)
{
	/*NO-OP*/
}