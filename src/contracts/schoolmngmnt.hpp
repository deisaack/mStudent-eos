#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/time.hpp>
#include <math.h>
#include <eosiolib/transaction.hpp>

using namespace eosio;
using namespace std;

CONTRACT schoolmngmnt : public eosio::contract
{
  public:
    schoolmngmnt(name self, name code, datastream<const char *> ds)
        : contract(self, code, ds), _schools(self, self.value), _students(self, self.value), _staffs(self, self.value), _stranfers(self, self.value), _indiscplines(self, self.value), _marks(self, self.value)
    {
    }

    /* 
      Actions
    */
    ACTION initstudent(eosio::name student, string full_name, eosio::name school, eosio::name creator);                                               // Initialize a student instance
    ACTION initschool(eosio::name school, string location, string long_name, string category, string email, eosio::name creator);                                       // Initialiaze institution instance
    ACTION addmark(eosio::name student, string subject, int64_t score, eosio::name staff);                                                            // Update student record(s)
    ACTION inittransfer(eosio::name student, eosio::name to_school, eosio::name staff);                                                               // Initialize a student transfer
    ACTION approvetrans(eosio::name student, eosio::name approver, eosio::name institution);                                                          // Approve a transfer
    ACTION discipline(eosio::name & student, string & action, string & comment, time_point_sec starttime, time_point_sec endtime, eosio::name staff); // Disciplinary Action against a student?

  private:
    /*
        Private Declarations : TABLES and Private functions.
    */

    TABLE school
    {
        uint64_t id;
        eosio::name school;
        string location;
        string longname;
        string category;
        string email;
        eosio::name creator;

        // uint64_t primary_key() const { return id; }
        uint64_t primary_key() const {return school.value;}
    };
    typedef eosio::multi_index<"schools"_n, school> schools_index;

    TABLE student
    {
        uint64_t id;
        eosio::name student;
        string fullname;
        eosio::name creator;
        eosio::name school;

        uint64_t primary_key() const { return student.value; }
    };
    typedef eosio::multi_index<"students"_n, student> students_index;

    TABLE markentry
    {
        uint64_t id;
        eosio::name student;
        string subject;
        int64_t score;
        uint64_t primary_key() const { return student.value;}
    };
    typedef eosio::multi_index<"marks"_n, markentry> marks_index;

    TABLE staff
    {
        uint64_t id;
        eosio::name staff;
        string fullname;

        uint64_t primary_key() const { return staff.value; }
    };
    typedef eosio::multi_index<"staffs"_n, staff> staffs_index;

    TABLE stransfer
    {
        uint64_t id;
        eosio::name student;
        string comment;
        bool approved_from;
        bool approved_to;
        eosio::name from;
        eosio::name to;
        eosio::time_point_sec init;

        uint64_t primary_key() const { return student.value; }
    };
    typedef eosio::multi_index<"stransfers"_n, stransfer> transfers_index;

    TABLE indiscipline
    {
        uint64_t id;
        eosio::name student;
        eosio::name official;
        string reason;
        string comment;

        uint64_t primary_key() const { return student.value; }
    };
    typedef eosio::multi_index<"indiscplines"_n, indiscipline> indiscplines_index;

    schools_index _schools;
    students_index _students;
    marks_index _marks;
    staffs_index _staffs;
    transfers_index _stranfers;
    indiscplines_index _indiscplines;

    void check_membership(eosio::name staff, eosio::name school)
    {
        // TODO - Implementation for checking if `staff` is a member of `school`
    }
};
