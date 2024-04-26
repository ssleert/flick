import css from "./Registration.module.less";
import FlickLogoSrc from "#shared/img/flick-logo.svg";
import MailIconSrc from "#shared/img/mail-icon.svg";
import LockIconSrc from "#shared/img/lock-icon.svg";

const Registration = () => {
  return <div class={css.Page}>
    <div class={css.Registration}>
      <div class={css.Window}>
        <div class={css.Title}>
          <img src={FlickLogoSrc} />
          <h3 class={css.Text}>
            Flick
          </h3>
        </div>

        <p class={css.Info}>
          Account Creation
        </p>

        <div class={css.InputFields}>
          <div class={css.Field}>
            <img src={MailIconSrc} />
            <input type="text"
                   placeholder="nickname" />
          </div>

          <div class={css.Field}>
            <img src={MailIconSrc} />
            <input type="text"
                   placeholder="example@mail.com" />
          </div>

          <div class={css.Field}>
            <img src={LockIconSrc} />
            <input type="password"
                   placeholder="password" />
          </div>
        </div>

        <button class={css.Button}>
          Register
        </button>
      </div>
    </div>
  </div>
}

export default Registration;
