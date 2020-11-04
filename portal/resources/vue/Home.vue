<template>
  <div>
    <Navigation></Navigation>
    <h1>Extensions List</h1>

    <table class="table mt-4 table-striped">
      <thead>
        <tr>
          <th scope="col">#</th>
          <th scope="col">ID</th>
          <th scope="col">Name</th>
          <th scope="col">Actions</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="extension in extensions" v-bind:key="extension.id">
          <th scope="row">{{ extension.index }}</th>
          <td scope="row">{{ extension.id }}</td>
          <td>{{ extension.name }}</td>
          <td>
            <router-link
              :to="{ name: 'sessions', params: { id: extension.id } }"
              >Sessions</router-link
            >
          </td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<script>
import ordinal from "ordinal";
import Navigation from "../vue/Navigation";

export default {
  data() {
    return {
      extensions: [],
    };
  },
  mounted() {
    window.axios.get("/api/extensions").then((response) => {
      this.extensions = response.data;

      let i = 1;
      this.extensions.forEach((extension) => {
        extension.index = ordinal(i);
        i += 1;
      });
    });
  },
  components: {
    Navigation,
  },
  props: ["selection"],
};
</script>