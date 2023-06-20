import {
  Daily,
  Friday,
  Monday,
  Saturday,
  Sunday,
  Thursday,
  Tuesday,
  Wednesday,
  Weekly,
} from "./frequencies.js";
import { formTypeHandler } from "./handlers.js";
import { renderSchedule, startLoading, stopLoading } from "./schedule.js";
import { addSchedule } from "./service.js";

export const renderForm = () => {
  const form = document.createElement("div");
  form.className = "schedule-form";

  //Type input
  const typeInput = document.createElement("select");
  typeInput.addEventListener("change", (e) => formTypeHandler(e.target.value));
  typeInput.className = "type-input";

  const typeInputDAILY = document.createElement("option");
  typeInputDAILY.value = Daily;
  typeInputDAILY.innerText = Daily;

  const typeInputWEEKLY = document.createElement("option");
  typeInputWEEKLY.value = Weekly;
  typeInputWEEKLY.innerText = Weekly;
  typeInput.appendChild(typeInputDAILY);
  typeInput.appendChild(typeInputWEEKLY);

  //Day input
  const dayInput = document.createElement("select");
  dayInput.className = "day-input";
  const dayInputSunday = document.createElement("option");
  dayInputSunday.value = Sunday;
  dayInputSunday.innerText = Sunday;

  const dayInputMonday = document.createElement("option");
  dayInputMonday.value = Monday;
  dayInputMonday.innerText = Monday;

  const dayInputTuesday = document.createElement("option");
  dayInputTuesday.value = Tuesday;
  dayInputTuesday.innerText = Tuesday;

  const dayInputWednesday = document.createElement("option");
  dayInputWednesday.value = Wednesday;
  dayInputWednesday.innerText = Wednesday;

  const dayInputThursday = document.createElement("option");
  dayInputThursday.value = Thursday;
  dayInputThursday.innerText = Thursday;

  const dayInputFriday = document.createElement("option");
  dayInputFriday.value = Friday;
  dayInputFriday.innerText = Friday;

  const dayInputSaturday = document.createElement("option");
  dayInputSaturday.value = Saturday;
  dayInputSaturday.innerText = Saturday;

  dayInput.appendChild(dayInputSunday);
  dayInput.appendChild(dayInputMonday);
  dayInput.appendChild(dayInputTuesday);
  dayInput.appendChild(dayInputWednesday);
  dayInput.appendChild(dayInputThursday);
  dayInput.appendChild(dayInputFriday);
  dayInput.appendChild(dayInputSaturday);

  const hourInput = document.createElement("input");
  hourInput.className = "hour-input";
  hourInput.placeholder = "HH";

  const minuteInput = document.createElement("input");
  minuteInput.className = "minute-input";
  minuteInput.placeholder = "mm";

  const addButton = document.createElement("div");
  addButton.className = "add-button";
  addButton.innerHTML = "&plus;";

  addButton.addEventListener("click", async (_) => {
    startLoading();

    await addSchedule(
      typeInput.value,
      dayInput.value,
      `${hourInput.value}:${minuteInput.value}`
    );
    await renderSchedule();
    stopLoading();
  });
  form.appendChild(typeInput);
  form.appendChild(dayInput);
  form.appendChild(hourInput);
  form.appendChild(minuteInput);

  form.appendChild(addButton);

  return form;
};
