import { Daily } from "./frequencies.js";

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
  const numeric = el.value.replace(/\D/g, "");
  el.value = numeric;
};
