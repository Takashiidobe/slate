struct member_tag_guard {
  struct member_tag_easy *data;
  struct member_tag_outer {
    struct member_tag_nested *nested;
  } outer;
};
void member_tag_enter(struct member_tag_easy *data);
void member_tag_nested_enter(struct member_tag_nested *nested);
