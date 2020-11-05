<template>
  <div>
    <Navigation :session="session" :extension="session.extension"></Navigation>
    <h1>Generations of Session {{ session.id }}</h1>

    <table
      class="table mt-4 table-striped"
      v-if="generations.length > 0 && loaded"
    >
      <thead>
        <tr>
          <th scope="col">#</th>
          <th scope="col">ID</th>
          <th scope="col">Actions</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="generation in generations" v-bind:key="generation.id">
          <th scope="row">{{ generation.index }}</th>
          <td scope="row">{{ generation.id }}</td>
          <td>
            <router-link
              :to="{
                name: 'generation_scores',
                params: { id: generation.id },
              }"
              >Scores</router-link
            >
          </td>
        </tr>
      </tbody>
    </table>
    <ItemNotFound v-if="generations.length == 0 && loaded" />
  </div>
</template>

<script>
import ordinal from "ordinal";
import ItemNotFound from "./ItemNotFound";
import Navigation from "../vue/Navigation";

export default {
  data() {
    return {
      session: {},
      generations: [],
      loaded: false,
    };
  },
  mounted() {
    window.axios
      .get("/api/generations/" + this.$route.params.id)
      .then((response) => {
        const data = response.data;
        this.generations = data.generations;
        this.session = data.session;

        let i = this.generations.length;
        this.generations.forEach((generation) => {
          generation.index = ordinal(i);
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