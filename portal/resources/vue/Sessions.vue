<template>
  <div>
    <Navigation :extension="extension"></Navigation>
    <h1>Sessions of {{ extension.name }}</h1>
    <table
      class="table mt-4 table-striped"
      v-if="sessions.length > 0 && loaded"
    >
      <thead>
        <tr>
          <th scope="col">#</th>
          <th scope="col">ID</th>
          <th scope="col">Actions</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="session in sessions" v-bind:key="session.id">
          <th scope="row">{{ session.index }}</th>
          <td scope="row">{{ session.id }}</td>
          <td>
            <router-link
              :to="{ name: 'generations', params: { id: session.id } }"
              >Generations</router-link
            >
            &nbsp;
            <router-link
              :to="{ name: 'session_scores', params: { id: session.id } }"
              >Scores</router-link
            >
          </td>
        </tr>
      </tbody>
    </table>
    <ItemNotFound v-if="sessions.length == 0 && loaded" />
  </div>
</template>

<script>
import ordinal from "ordinal";
import ItemNotFound from "./ItemNotFound";
import Navigation from "../vue/Navigation";

export default {
  data() {
    return {
      sessions: [],
      extension: {},
      loaded: false,
    };
  },
  mounted() {
    window.axios
      .get("/api/sessions/" + this.$route.params.id)
      .then((response) => {
        const data = response.data;
        this.sessions = data.sessions;
        this.extension = data.extension;
        let i = this.sessions.length;
        this.sessions.forEach((session) => {
          session.index = ordinal(i);
          i -= 1;
        });
        this.loaded = true;
      });
  },
  components: {
    ItemNotFound,
    Navigation,
  },
  props: ["selection"],
};
</script>