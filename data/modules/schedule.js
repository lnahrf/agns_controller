import { renderForm } from "./form.js";
import { formatSchedule, formatScheduleObj } from "./format.js";
import { Daily, Weekly } from "./enums.js";
import { formButtonHandler, formTypeHandler } from "./handlers.js";
import { getSchedule, removeSchedule } from "./service.js";

export const renderScheduleItem = (scheduleStr) => {
  const scheduleObj = formatScheduleObj(scheduleStr);
  const wrapper = document.createElement("div");

  wrapper.className = "schedule-list-item-wrapper";
  let typeDiv;
  let durationDiv;
  let hourDiv;
  let dayDiv;

  switch (scheduleObj.type) {
    case Daily:
      typeDiv = document.createElement("div");
      typeDiv.className = "schedule-list-item-type";
      typeDiv.innerText = scheduleObj.type;

      durationDiv = document.createElement("div");
      durationDiv.className = "schedule-list-item-duration";
      durationDiv.innerText = scheduleObj.duration + " Min";

      hourDiv = document.createElement("div");
      hourDiv.className = "schedule-list-item-hour";
      hourDiv.innerText = scheduleObj.hour;

      wrapper.appendChild(typeDiv);
      wrapper.appendChild(hourDiv);
      wrapper.appendChild(durationDiv);

      break;
    case Weekly:
      typeDiv = document.createElement("div");
      typeDiv.className = "schedule-list-item-type";
      typeDiv.innerText = scheduleObj.type;

      durationDiv = document.createElement("div");
      durationDiv.className = "schedule-list-item-duration";
      durationDiv.innerText = scheduleObj.duration + " Min";

      hourDiv = document.createElement("div");
      hourDiv.className = "schedule-list-item-hour";
      hourDiv.innerText = scheduleObj.hour;

      dayDiv = document.createElement("div");
      dayDiv.className = "schedule-list-item-day";
      dayDiv.innerText = scheduleObj.day;

      wrapper.appendChild(typeDiv);
      wrapper.appendChild(dayDiv);
      wrapper.appendChild(hourDiv);
      wrapper.appendChild(durationDiv);
      break;
    default:
      break;
  }

  const deleteBtn = document.createElement("div");
  deleteBtn.className = "schedule-list-item-delete";
  deleteBtn.innerHTML = "&#10006;";

  deleteBtn.addEventListener("click", async (_) => {
    startLoading();
    await removeSchedule(scheduleStr);
    await renderSchedule();
    stopLoading();
  });

  wrapper.appendChild(deleteBtn);

  const li = document.createElement("li");
  li.className = "schedule-list-item";
  li.appendChild(wrapper);

  return li;
};

export const startLoading = () => {
  const root = document.getElementById("root");
  root.style.pointerEvents = "none";
  root.style.opacity = "0.5";
  root.style.cursor = "not-allowed";
};

export const stopLoading = () => {
  const root = document.getElementById("root");
  root.style.pointerEvents = "inherit";
  root.style.opacity = "1";
  root.style.cursor = "auto";
};

export const renderSchedule = async () => {
  const root = document.getElementById("root");

  startLoading();

  root.innerHTML = "";
  const list = document.createElement("ul");
  list.className = "schedule-list";
  const scheduleStr = await getSchedule();
  const formatted = await formatSchedule(scheduleStr);

  for (const schedule of formatted) {
    const li = renderScheduleItem(schedule);
    list.appendChild(li);
  }

  const form = renderForm();

  root.appendChild(list);
  root.appendChild(form);
  formTypeHandler(Daily);
  formButtonHandler();

  stopLoading();
};
