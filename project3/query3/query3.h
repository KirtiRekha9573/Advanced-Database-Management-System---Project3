#pragma once

#include <stdio.h>
#include <iostream>

// Structure definitions
struct DATE
{
    int32_t day;
    int32_t month;
    int32_t year;
};

#pragma pack(push, 1)
struct Person
{
    char Fname[20];
    char Lname[20];
    char job[70];
    char company[40];
    char address[80];
    char phone[26];
    DATE bdate;
    char ssn[12];
    char username[25];
    char email[50];
    char url[50];
};
#pragma pack(pop)