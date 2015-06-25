#include <windows.h>
#include <stdio.h>

void main() {

   LARGE_INTEGER liFrequency;
   LARGE_INTEGER liCurrent;
   LARGE_INTEGER liLast;
   LARGE_INTEGER liRecent[10];
   DWORD dwCurrent;
   DWORD dwLast;
   DWORD dwPerfElapsed;
   DWORD dwTickElapsed;
   int i = 0;
   int j;

   // Save the performance counter frequency for later use.
   if (!QueryPerformanceFrequency(&liFrequency))
      printf("QPF() failed with error %d\n", GetLastError());

   // Query the performance counter value and tick count.
   dwCurrent = GetTickCount();
   if (!QueryPerformanceCounter(&liCurrent))
      printf("QPC() failed with error %d\n", GetLastError());

   liLast = liCurrent;
   dwLast = dwCurrent;

   while (TRUE) {

      // Query the performance counter value and tick count.
      if (!QueryPerformanceCounter(&liCurrent))
         printf("QPC() failed with error %d\n", GetLastError());
      dwCurrent = GetTickCount();

      // Store the performance counter value in the list of recent values.
      liRecent[i].QuadPart = liCurrent.QuadPart;
      i = (i+1) % 10;

      // Convert difference in performance counter values to milliseconds.
      dwPerfElapsed = (DWORD) (((liCurrent.QuadPart - liLast.QuadPart) * 1000) / liFrequency.QuadPart);
      
      dwTickElapsed = dwCurrent - dwLast;

      // Check for a discrepancy greater than 500 milliseconds.
      if (abs(dwPerfElapsed - dwTickElapsed) > 500) { 

         // Print the previous 9 performance-counter values.
         for (j=9; j>0; j--) {
            printf("      Previous %d:  %I64X\n", j, liRecent[i].QuadPart);
            i = (i+1) % 10;
         }

         // Print the leap value.
         printf( "LEAP: Current:     %I64X  delta = %I64X\n", 
               liCurrent.QuadPart, liCurrent.QuadPart - liLast.QuadPart);
         
         // Retrieve and print the next 9 performance-counter values.
         for (j=1; j<=9; j++) {
            
            QueryPerformanceCounter(&liCurrent);
            printf("      Next     %d:  %I64X\n", j, liCurrent.QuadPart);
            
            liRecent[i].QuadPart = liCurrent.QuadPart;
            i = (i+1) % 10;
         }

         printf("\n");
      }

      liLast = liCurrent;
      dwLast = dwCurrent;
   }

   printf("Finished.");
   return;
}