use rocket::{response::content, form::Form};

#[get("/auth")]
pub fn auth() -> content::RawHtml<&'static str>
{
    content::RawHtml(
        r#"
            <h1>Account Creation</h1>
            <form action="/register_account" method="post">
                <input type="text" name="login" id="login"><br>
                <input type="password" id="password"><br>
                <input type="submit" value="Register"><br>
            </form>
        "#
    )
}


#[derive(FromForm)]
pub struct Account
{
    login: String,
}

#[post("/register_account", data = "<account>")]
pub fn register_account(account: Form<Account>) -> content::RawHtml<String>
{
    let is_success = false;
    let asd = "asd";

    let reponse: String = match is_success {
        true => {
            format!(
                r#"
                    <h1>Account Creation</h1>
                    <form action="/register_account" method="post">
                        <input type="text" name="login" id="login"><br>
                        <input type="password" id="password"><br>
                        <input type="submit" value="Register"><br>
                    </form>
                "#
            )
        },

        false => {
            format!(
                r#"
                    <h1>Account Creation</h1>
                    <form action="/register_account" method="post">
                        <input type="text" name="login" id="login"><br>
                        <input type="password" id="password"><br>
                        <input type="submit" value="Register"><br>
                        {asd}
                        <p style="red">Email is already registered</p>
                    </form>
                "#
            )
        }
    };


    content::RawHtml(reponse)
}
