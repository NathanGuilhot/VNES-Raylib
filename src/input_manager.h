//Input manager

bool a_pressed = false; //A
bool b_pressed = false; //B
bool u_pressed = false; //UP
bool d_pressed = false; //DOWN
bool l_pressed = false; //LEFT (gauche)
bool r_pressed = false; //RIGHT (drouate)
bool start_pressed = false;

bool BTN(char *pKey)
{ //Maybe replace the string by an enum?
  if (strcmp(pKey, "A") == 0)
  {
    a_pressed = (IsKeyDown(KEY_SPACE));
    return a_pressed;
  }
  else if (strcmp(pKey, "B") == 0)
  {
    b_pressed = (IsKeyDown(KEY_ESCAPE));
    return b_pressed;
  }
  else if (strcmp(pKey, "UP") == 0)
  {
    u_pressed = (IsKeyDown(KEY_UP));
    return u_pressed;
  }
  else if (strcmp(pKey, "DOWN") == 0)
  {
    d_pressed = (IsKeyDown(KEY_DOWN));
    return d_pressed;
  }
  else if (strcmp(pKey, "LEFT") == 0)
  {
    l_pressed = (IsKeyDown(KEY_LEFT));
    return l_pressed;
  }
  else if (strcmp(pKey, "RIGHT") == 0)
  {
    r_pressed = (IsKeyDown(KEY_RIGHT));
    return r_pressed;
  }
  else if (strcmp(pKey, "START") == 0)
  {
    start_pressed = (IsKeyDown(KEY_ENTER));
    return start_pressed;
  }
  else
  {
    return false;
  }
}

bool BTNP(char *pKey)
{

  if (strcmp(pKey, "A") == 0)
  {
    if (!a_pressed)
    {
      return BTN(pKey);
    }
    else
    {
      BTN(pKey);
    }
  }
  else if (strcmp(pKey, "B") == 0)
  {
    if (!b_pressed)
    {
      return BTN(pKey);
    }
    else
    {
      BTN(pKey);
    }
  }
  else if (strcmp(pKey, "UP") == 0)
  {
    if (!u_pressed)
    {
      return BTN(pKey);
    }
    else
    {
      BTN(pKey);
    }
  }
  else if (strcmp(pKey, "DOWN") == 0)
  {
    if (!d_pressed)
    {
      return BTN(pKey);
    }
    else
    {
      BTN(pKey);
    }
  }
  else if (strcmp(pKey, "LEFT") == 0)
  {
    if (!l_pressed)
    {
      return BTN(pKey);
    }
    else
    {
      BTN(pKey);
    }
  }
  else if (strcmp(pKey, "RIGHT") == 0)
  {
    if (!r_pressed)
    {
      return BTN(pKey);
    }
    else
    {
      BTN(pKey);
    }
  }
  else if (strcmp(pKey, "START") == 0)
  {
    if (!start_pressed)
    {
      return BTN(pKey);
    }
    else
    {
      BTN(pKey);
    }
  }

  return false;
}