
#ifndef __GETLINE_H_
#define __GETLINE_H_



///
/// get_line
///
/// for purpose of compatability with Windows mingw,
/// this function has a similar approach to
/// getline found in the POSIX library
///
/// src: https://riptutorial.com/c/example/8274/get-lines-from-a-file-using-getline--
///
/// @param buf 		buffer to store line
/// @param size		the initial size pointer to read
/// @param stream	where to read info from
/// @return			the number of bytes read
long int get_line( char** buf, size_t* size, FILE* stream ); 


#endif //__GETLINE_H_
