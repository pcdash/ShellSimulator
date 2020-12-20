///////////////////////////////////////////////////////////////////////////////
/// \file         main.c
/// \author       Paul Valdez
/// \date         December 6, 2020
/// \brief        Shell simulator that can affect files on computer.
///
///
/// REVISION HISTORY:
/// \date  12/06/2020 (created)
///
///////////////////////////////////////////////////////////////////////////////

#include "shell.h"

int main(int argc, char *argv[], char *env[])
{
    myShell(argc, argv, env);
    return 0;
}