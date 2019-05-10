
#ifndef __GETLINE_H_
#define __GETLINE_H_


/// getline
///
/// acts the same way as getline from POSIX,
/// for the purpose of compatibility,
///
/// link: https://riptutorial.com/c/example/8274/get-lines-from-a-file-using-getline--
///
/// @param buf 		buffer to store line
/// @param size		the initial size pointer to read
/// @param stream	where to read info from
/// @return			the number of bytes read
long int getline( char** buf, size_t* size, FILE* stream ); 

#endif //__GETLINE_H_
