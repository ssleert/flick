#[macro_use] extern crate rocket;
use rocket::{Rocket, Build};

mod auth;


#[launch]
fn rocket() -> Rocket<Build>
{
    rocket::build().mount("/", routes![auth::auth, auth::register_account])
}
