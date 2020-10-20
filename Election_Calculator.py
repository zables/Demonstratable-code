# Import libraries

import requests
from datetime import date
from bs4 import BeautifulSoup
import numpy as np
import matplotlib.pyplot as plt

today = date.today()
today = today.strftime("%Y-%m-%d")
print("The date is: " + today)

cur_year = int(today[0:4])
cur_month = int(today[5:7])
cur_day = int(today[8:10])

biden_values = []
trump_values = []

states = ['alabama', 'alaska', 'arizona', 'arkansas', 'california', 'colorado', 'connecticut', 'delaware',
          'district-of-columbia', 'florida', 'georgia', 'hawaii',
          'idaho', 'illinois', 'indiana', 'iowa', 'kansas', 'kentucky', 'louisiana', 'maine', 'maryland',
          'massachusetts', 'michigan', 'minnesota', 'mississippi',
          'missouri', 'montana', 'nebraska', 'nevada', 'new-hampshire', 'new-jersey', 'new-mexico', 'new-york',
          'north-carolina', 'north-dakota', 'ohio',
          'oklahoma', 'oregon', 'pennsylvania', 'rhode-island', 'south-carolina', 'south-dakota', 'tennessee', 'texas',
          'utah', 'vermont', 'virginia',
          'washington', 'west-virginia', 'wisconsin', 'wyoming']
state_value = [9, 3, 11, 6, 55, 9, 7, 3, 3, 29, 16, 4, 4, 20, 11,
               6, 6, 8, 8, 4, 10, 11, 16, 10, 6, 10, 3,
               5, 6, 4, 14, 5, 29, 15, 3, 18, 7, 7, 20,
               4, 9, 3, 11, 38, 6, 3, 13, 12, 5, 10, 3]
# if a state has no data, then the state is not included in my tests, despite previous voting outcomes in the
# previous election
num_removed = 0
s = 0
while s < len(states):
    data_points = 0
    # Set the URL you want to web scrape from
    url = 'https://projects.fivethirtyeight.com/polls/president-general/' + states[s] + '/'
    # Connect to the URL
    response = requests.get(url)

    # Parse HTML and save to BeautifulSoup object
    soup = BeautifulSoup(response.text, "html.parser")

    body = soup.body
    if body is None:
        print("\nNo data for " + states[s] + "\n")
        states.pop(s)
        state_value.pop(s)
        continue

    polls = body.find('div', class_="polls")
    biden_total = 0
    trump_total = 0
    for day_container in polls.find_all('div', class_='day-container'):
        if data_points >= 5:
            break
        date = day_container.h2['data-date']
        data_year = int(date[0:4])
        data_month = int(date[5:7])
        data_day = int(date[8:10])

        # This is an idea to implement later to get a more exact date and time cutoff
        # date_value = data_year * 365 + data_month * 30 + data_day
        # today_value = cur_year * 365 + cur_month * 30 + cur_day

        # if greater than 2 months old, break
        if abs(cur_month - data_month) > 2 and data_points > 0:
            break

        print("{:<20}".format(states[s]) + date)
        polls_table = day_container.table

        t_body = polls_table.tbody

        for visible_row in t_body.find_all('tr', 'visible-row'):
            if data_points >= 5:
                break
            name_box = visible_row.find('td', 'answer first hide-mobile')

            if name_box is None:
                continue
            name_1 = name_box.text

            name_box2 = visible_row.find('td', 'answer hide-mobile')

            if name_box2 is None:
                continue
            name_2 = name_box2.text

            if name_1 == "Biden" and name_2 == "Trump":
                data_points += 1

                final_step = visible_row.find('td', 'value hide-mobile')

                percent_text = final_step.text

                percent_text = percent_text[:-1]

                biden_total += int(percent_text)

                final_step = final_step.find_next_sibling('td', 'value hide-mobile')
                percent_text = final_step.text

                percent_text = percent_text[:-1]
                trump_total += int(percent_text)

    if data_points != 0:
        biden_total /= data_points
        trump_total /= data_points
    trump_values.append(trump_total)
    biden_values.append(biden_total)
    s += 1

num_biden_wins = 0
num_trump_wins = 0
num_ties = 0
biden_delegates = 0
trump_delegates = 0

num_close = 0
num_close_delegates = 0
close_states = []
close_state_delegates = []
close_state_margin = []
tie_delegates = 0
blue_states = []
blue_state_values = []
red_states = []
red_state_values = []

print("{:<20}".format("States") + "{:}".format("Biden %") + "{:>11}".format("Trump %") + "{:>8}".format("Net"))
for b in range(0, len(biden_values)):
    print("{:<20}".format(states[b]) + "{:.2f}".format(biden_values[b]) + "%" + "{:>10.2f}".format(
        trump_values[b]) + "%" + "{:>10.2f}".format(biden_values[b] - trump_values[b]))
    if abs(biden_values[b] - trump_values[b]) <= 5:
        num_close += 1
        num_close_delegates += state_value[b]
        close_states.append(states[b])
        close_state_delegates.append(state_value[b])
        close_state_margin.append(biden_values[b] - trump_values[b])
    if abs(biden_values[b] - trump_values[b]) <= 1:
        num_ties += 1
        tie_delegates += state_value[b]
    elif biden_values[b] > trump_values[b]:
        num_biden_wins += 1
        biden_delegates += state_value[b]
        if biden_values[b] - trump_values[b] >= 8:
            blue_states.append(states[b])
            blue_state_values.append(state_value[b])
    else:
        num_trump_wins += 1
        trump_delegates += state_value[b]
        if biden_values[b] - trump_values[b] <= -8:
            red_states.append(states[b])
            red_state_values.append(state_value[b])
print("Biden wins \t Ties\t Trump Wins\n" + "\t" + str(num_biden_wins) + "\t\t\t" + str(num_ties) + "\t\t" + str(
    num_trump_wins))

print("Biden Delegates\t\tTie delegates\t\tTrump Delegates\n" + str(biden_delegates) + "\t\t\t\t\t"+str(tie_delegates) +
      "\t\t\t" + str(trump_delegates))

print("number of swing states: " + str(num_close))

print("number of delegates for swing states: " + str(num_close_delegates))

print("\nPositive means in favor of Biden, negative means in favor of Trump")
trump_swing_state_delegate = 0
biden_swing_state_delegate = 0
for i in range(0, len(close_states)):
    print("{:<15}".format(close_states[i]) + "\t" + "{:.2f}".format(close_state_margin[i]))
    if close_state_margin[i] < 0:
        trump_swing_state_delegate += close_state_delegates[i]
    if close_state_margin[i] > 0:
        biden_swing_state_delegate += close_state_delegates[i]
print("Biden swing delegates:", biden_swing_state_delegate)
print("Trump swing delegates:", trump_swing_state_delegate)
total = 0
print("\nThese blue states have a lead of 8 or greater and show corresponding delegate value")
for i in range(0, len(blue_states)):
    print("{:<15}".format(blue_states[i]) + str(blue_state_values[i]))
    total += blue_state_values[i]
print("Total\t" + str(total))
print("Total States\t" + str(len(blue_states)))

total = 0
print("\nThese red states have a lead of 8 or greater and show corresponding delegate value")
for i in range(0, len(red_states)):
    print("{:<15}".format(red_states[i]) + str(red_state_values[i]))
    total += red_state_values[i]
print("Total\t" + str(total))
print("Total States\t" + str(len(red_states)))
objects = ('Biden', 'Trump')
y_pos = np.arange(len(objects))

biden_bar = plt.bar('Biden', biden_delegates, alpha=1, color='b', width=.5)
trump_bar = plt.bar('Trump', trump_delegates, align='edge', alpha=1, color='r', width=.5)
plt.xticks(y_pos, objects)
plt.ylabel('Delegates')
plt.title('Delegate Split')
plt.axhline(270, c='black')

plt.show()
"""
print("\n\nThis is the current combination of states needed to win from the tossups, given that other states will go to 
that candidate if it has a lean")
biden_lean = 0
trump_lean = 0
for i in range(0,len(biden_values)):
    if (biden_values[i] - trump_values[i] > 5):
        biden_lean += state_value[i]
    if trump_values[i]-biden_values[i] > 5:
        trump_lean+= state_value[i]
num_combos = 0
print("Biden combos")
"""
