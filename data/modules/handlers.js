import { Daily } from "./enums.js";

export const formTypeHandler = (type) => {
  if (type === Daily)
    document.getElementsByClassName("day-input")[0].style.display = "none";
  else {
    document.getElementsByClassName("day-input")[0].style.display = "inline";
  }
};

export const formButtonHandler = () => {
  const btn = document.getElementsByClassName("add-button")[0];
  const hourInput = document.getElementsByClassName("hour-input")[0];
  const minuteInput = document.getElementsByClassName("minute-input")[0];

  if (!hourInput.value || !minuteInput.value) {
    btn.style.pointerEvents = "none";
    btn.style.opacity = "0.5";
    btn.style.cursor = "not-allowed";
  } else {
    btn.style.pointerEvents = "all";
    btn.style.opacity = "1";
    btn.style.cursor = "pointer";
  }
};

export const formTimeHandler = (el) => {
  const className = el.className;
  const threshold = className === "minute-input" ? 59 : 23;
  const numeric = el.value.replace(/\D/g, "");
  const parsed = Number(numeric);
  el.value =
    parsed < 10 ? "0" + parsed : parsed > threshold ? threshold : parsed;
};
