/* $Id: del_test2_p.c,v 1.2 2002/12/11 22:16:26 speck Exp $ */

/*
 * del_test2_p.c:
 *
 * This application demonstrates/tests the sensitivity of the mc_delete_family()
 * call to write access by the deleting process or a different process.  It
 * is intended to be run in synchronization with del_test2_s on a shared
 * file system on two different hosts (heterogeneous OS types OK).  del_test2_p
 * is intended to be executed first, followed in less than 10 seconds by
 * del_test2_s.
 * 
 * This work was produced at the University of California, Lawrence 
 * Livermore National Laboratory (UC LLNL) under contract no. 
 * W-7405-ENG-48 (Contract 48) between the U.S. Department of Energy 
 * (DOE) and The Regents of the University of California (University) 
 * for the operation of UC LLNL. Copyright is reserved to the University 
 * for purposes of controlled dissemination, commercialization through 
 * formal licensing, or other disposition under terms of Contract 48; 
 * DOE policies, regulations and orders; and U.S. statutes. The rights 
 * of the Federal Government are reserved under Contract 48 subject to 
 * the restrictions agreed upon by the DOE and University as allowed 
 * under DOE Acquisition Letter 97-1.
 * 
 * 
 * DISCLAIMER
 * 
 * This work was prepared as an account of work sponsored by an agency 
 * of the United States Government. Neither the United States Government 
 * nor the University of California nor any of their employees, makes 
 * any warranty, express or implied, or assumes any liability or 
 * responsibility for the accuracy, completeness, or usefulness of any 
 * information, apparatus, product, or process disclosed, or represents 
 * that its use would not infringe privately-owned rights.  Reference 
 * herein to any specific commercial products, process, or service by 
 * trade name, trademark, manufacturer or otherwise does not necessarily 
 * constitute or imply its endorsement, recommendation, or favoring by 
 * the United States Government or the University of California. The 
 * views and opinions of authors expressed herein do not necessarily 
 * state or reflect those of the United States Government or the 
 * University of California, and shall not be used for advertising or 
 * product endorsement purposes.
 * 
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mili.h"

#ifndef OK
#define OK 0
#endif


main( int argc, char *argv[] )
{
    Famid fid;
    int rval;
    char *del_root = "delme";
    struct stat st;
    char pathname[64];

    /* Open a db with write access. */
    rval = mc_open( del_root, "data", "Aw", &fid );

    if ( rval == OK )
    {

        sleep( 10 );

        sprintf( pathname, "data/%sA", del_root );

        rval = stat( pathname, &st );

        if ( rval == 0 )
        {
            rval = mc_delete_family( del_root, "data" );

            if ( rval != OK )
                mc_print_error( "Unable to delete family after sleep", 
                                rval );
            else
                fprintf( stderr, "Primary process successfully deleted family "
                         "after sleep.\n" );
        }
        else
            fprintf( stderr, "Primary process failure - file \"%s\" not found "
                     "after sleep.\n", pathname );
    }
    else
        mc_print_error( "Unable to open family to init test", rval );

    exit( 0 );
}