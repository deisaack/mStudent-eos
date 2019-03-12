#include "schoolmngmnt.hpp"

ACTION schoolmngmnt::initstudent(eosio::name student, string full_name, eosio::name school, eosio::name creator)
{
    require_auth(creator); // Require that the actor signs the transaction. Some form of login_required check.

    // TODO - Ensure that the creator belongs to the `staff` table.

    // TODO - Check if `school` exists in schools table

    eosio_assert(is_account(student), "student should be a valid account"); // Check if student being added is a valid account.

    auto itr = _students.find(student.value);

    if (itr == _students.end()) // Student record does not exist yet. CREATE.
    {
        _students.emplace(get_self(), [&](auto &std) {
            std.id = _students.available_primary_key();
            std.student = student;
            std.fullname = full_name;
            std.creator = creator;
            std.school = school;
        });
    }
    else // Student exists already. MODIFY
    {
        _students.modify(itr, get_self(), [&](auto &std) {
            std.student = student;
            std.fullname = full_name;
            std.creator = creator;
            std.school = school;
        });
    }
}

ACTION schoolmngmnt::initschool(eosio::name school, string location, string long_name, string category, string email, eosio::name creator)
{
    require_auth(creator);
    auto itr = _schools.find(school.value); //Try to query for school given its account `name`
    if (itr == _schools.end())
    {
        // School with given name not found - CREATE
        _schools.emplace(get_self(), [&](auto &sch) {
            sch.id = _schools.available_primary_key();
            sch.school = school;
            sch.location = location;
            sch.longname = long_name;
            sch.category = category;
            sch.email = email;
            sch.creator = creator;
        });
    }
    else
    {
        // School with given name found. UPDATE/MODIFY
        _schools.modify(itr, get_self(), [&](auto &sch) {
            sch.school = school;
            sch.location = location;
            sch.longname = long_name;
            sch.category = category;
            sch.email = email;
            sch.creator = creator;
        });
    }
}

// ACTION schoolmngmnt::update(eosio::name student, string full_name, eosio::name staff)
// {
//     // Already taken care of above. IGNORE for now.
// }

ACTION schoolmngmnt::inittransfer(eosio::name student, eosio::name to_school, eosio::name staff)
{
    // Initiate a student transfer from School A to School B.
    require_auth(staff);
    // TODO - Ensure that the `staff` is in the school the student is enrolled in currently.
    _stranfers.emplace(get_self(), [&](auto &trf) {
        trf.id = _stranfers.available_primary_key();
        trf.student = student;
        trf.to = to_school;
    });
}

ACTION schoolmngmnt::addmark(eosio::name student, string subject, int64_t score, eosio::name staff)
{
    require_auth(staff);
    auto itr = _students.find(student.value);

    eosio_assert(itr != _students.end(), "was unable to find that student");

    _marks.emplace(get_self(), [&](auto & mrk){
        mrk.id = _marks.available_primary_key();
        mrk.student = student;
        mrk.subject = subject;
        mrk.score = score;
    });
}

ACTION schoolmngmnt::approvetrans(eosio::name student, eosio::name approver, eosio::name institution)
{
    require_auth(approver);

    // TODO - Check that `approver` is a staff member of either the `from_school` or the `to_school` using the `schoolmngmnt::check_membership` function
}

ACTION schoolmngmnt::discipline(eosio::name &student, string &action, string &comment, time_point_sec starttime, time_point_sec endtime, eosio::name staff)
{
    require_auth(staff);
}

extern "C" void apply(uint64_t receiver, uint64_t code, uint64_t action)
{
    if (code == "eosio.token"_n.value && action == "transfer"_n.value)
    {
        /* 
            Listen for token transfers. Useful if you wanna implement payments using crypto. 
            Students could pay fees and their balances could be updated in real time here.
            Or something else, fancy, of course :-)
        */
        eosio::print("Received some stufff from the student token contract. I am doing nothing much here, just having fun though!");
    }
    else if (code == receiver)
    {
        switch (action)
        {
            EOSIO_DISPATCH_HELPER(schoolmngmnt, (initstudent)(initschool)(addmark)(inittransfer)(approvetrans)(discipline))
        }
    }
}